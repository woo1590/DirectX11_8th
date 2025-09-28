#include "MapEditorPCH.h"
#include "Chunk.h"
#include "EngineCore.h"
#include "SpriteComponent.h"
#include "Shader.h"

Chunk::Chunk()
	:Object()
{
}

Chunk::Chunk(const Chunk& prototype)
	:Object(prototype)
{
}

Chunk* Chunk::Create()
{
	Chunk* Instance = new Chunk();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Chunk::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "Chunk";
	m_eRenderGroup = RenderGroup::Blend;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT Chunk::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	CHUNK_DESC* desc = static_cast<CHUNK_DESC*>(arg);

	m_pTransform->SetRotation(_float3(math::ToRadian(90.f), 0.f, 0.f));
	m_pTransform->SetScale(_float3(CHUNK_SIZE * CELL_SIZE, CHUNK_SIZE * CELL_SIZE, 1.f));
	m_pTransform->SetPosition(desc->chunkPosition);

	_float3 chunkPosition = m_pTransform->GetPosition();
	_float3 chunkMin{};
	chunkMin.x = chunkPosition.x - CELL_SIZE * CHUNK_SIZE * 0.5f;
	chunkMin.y = 0.f;
	chunkMin.z = chunkPosition.z - CELL_SIZE * CHUNK_SIZE * 0.5f;
	for(_uint i=0; i<CHUNK_SIZE; ++i)
		for (_uint j = 0; j < CHUNK_SIZE; ++j)
		{
			CELL cell{};
			cell.position.x = chunkMin.x + CELL_SIZE * j + CELL_SIZE / 2.f;
			cell.position.y = 0.f;
			cell.position.z = chunkMin.z + CELL_SIZE * i + CELL_SIZE / 2.f;

			m_Cells[i][j] = cell;
		}

	auto sprite = GetComponent<SpriteComponent>();
	sprite->SetBuffer(ENUM_CLASS(LevelID::Editor), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Editor), "Mtrl_Chunk");

	/*Create Constant Buffer*/
	auto device = EngineCore::GetInstance()->GetDevice();

	D3D11_BUFFER_DESC cbDesc{};
	cbDesc.ByteWidth = sizeof(CB_PER_CHUNK);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&cbDesc, nullptr, &m_pCBPerChunk)))
		return E_FAIL;

	/*Set Plane*/
	m_PlaneXZ = _float4(0.f, 1.f, 0.f, 0.f);

	return S_OK;
}

void Chunk::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Chunk::Update(_float dt)
{
	__super::Update(dt);
	

	UpdateConstantBuffer();
	BindConstantBuffer();

}

void Chunk::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);

}

Object* Chunk::Clone(InitDESC* arg)
{
	Chunk* Instance = new Chunk(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Chunk::Free()
{
	__super::Free();

	Safe_Release(m_pCBPerChunk);
}

void Chunk::UpdateConstantBuffer()
{
	auto engine = EngineCore::GetInstance();

	/*Compute HoverPosition (Plane - Ray Intersect)*/
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(engine->GetWindowHandle(), &mousePos);
	_float3 mouse{ static_cast<_float>(mousePos.x),static_cast<_float>(mousePos.y),0.f };
	_float4x4 projMat = engine->GetProjMatrix();
	_float4x4 viewMat = engine->GetViewMatrix();
	D3D11_VIEWPORT viewPort{};
	_uint numView = 1;
	engine->GetDeviceContext()->RSGetViewports(&numView,&viewPort);

	XMStoreFloat3(&mouse, XMVector3Unproject(XMVectorSetW(XMLoadFloat3(&mouse), 1.f),
		viewPort.TopLeftX, viewPort.TopLeftY, viewPort.Width, viewPort.Height,
		viewPort.MinDepth, viewPort.MaxDepth,
		XMLoadFloat4x4(&projMat),
		XMLoadFloat4x4(&viewMat),
		XMMatrixIdentity()));

	_float3 camPosition = engine->GetCamPosition();
	_float3 hoverPosition;
	XMStoreFloat3(&hoverPosition, XMPlaneIntersectLine(XMLoadFloat4(&m_PlaneXZ), XMLoadFloat3(&camPosition), XMLoadFloat3(&mouse)));

	/*Update Constant Buffer*/
	_float3 position = m_pTransform->GetPosition();

	D3D11_MAPPED_SUBRESOURCE cbPerChunkData{};
	CB_PER_CHUNK chunk{};
	chunk.chunkMin.x = position.x - CELL_SIZE * CHUNK_SIZE * 0.5f;
	chunk.chunkMin.y = 0.f;
	chunk.chunkMin.z = position.z - CELL_SIZE * CHUNK_SIZE * 0.5f;
	chunk.hoverPosition = hoverPosition;
	chunk.cellSize = CELL_SIZE;


	auto context = EngineCore::GetInstance()->GetDeviceContext();
	context->Map(m_pCBPerChunk, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbPerChunkData);
	memcpy_s(cbPerChunkData.pData, sizeof(CB_PER_CHUNK), &chunk, sizeof(CB_PER_CHUNK));
	context->Unmap(m_pCBPerChunk, 0);
}

HRESULT Chunk::BindConstantBuffer()
{
	/*Bind Constant Buffer*/
	auto shader = EngineCore::GetInstance()->GetShader("Shader_Chunk");

	return shader->SetConstantBuffer(m_pCBPerChunk, "PerChunk");
}
