#include "MapEditorPCH.h"
#include "Chunk.h"
#include "EngineCore.h"
#include "SpriteComponent.h"
#include "ChunkPickable.h"
#include "MaterialInstance.h"
#include "Shader.h"

Chunk::Chunk()
	:Object()
{
}

Chunk::Chunk(const Chunk& prototype)
	:Object(prototype),
	m_pPickingSystem(prototype.m_pPickingSystem)
{
	m_pPickingSystem->AddRef();
}

Chunk * Chunk::Create(PickingSystem* picking)
{
	Chunk* Instance = new Chunk();

	if (FAILED(Instance->Initialize_Prototype(picking)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Chunk::Initialize_Prototype(PickingSystem* picking)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "Chunk";
	m_eRenderGroup = RenderGroup::NonBlend;
	m_pPickingSystem = picking;
	m_pPickingSystem->AddRef();

	AddComponent<SpriteComponent>();
	AddComponent<ChunkPickable>();

	return S_OK;
}

HRESULT Chunk::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	CHUNK_DESC* desc = static_cast<CHUNK_DESC*>(arg);

	m_pTransform->Rotate(_float3(math::ToRadian(90.f), 0.f, 0.f));
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

	if (FAILED(sprite->Initialize(arg)))
		return E_FAIL;

	sprite->SetBuffer(ENUM_CLASS(LevelID::Editor), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Editor), "Mtrl_Chunk");


	/*Register PickingSystem*/
	m_pPickingSystem->RegisterComponent(GetComponent<ChunkPickable>());

	return S_OK;
}

void Chunk::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Chunk::Update(_float dt)
{
	__super::Update(dt);
	
	UpdatePlane();
	UpdateParams();

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
	m_pPickingSystem->UnRegisterComponent(GetComponent<ChunkPickable>());
	Safe_Release(m_pPickingSystem);
	__super::Free();

}

_float3 Chunk::GetHoverPosition()
{
	auto engine = EngineCore::GetInstance();
	CAMERA_CONTEXT camContext = engine->GetCameraContext();

	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(engine->GetWindowHandle(), &mousePos);
	_float3 mouse{ static_cast<_float>(mousePos.x),static_cast<_float>(mousePos.y),0.f };
	_float4x4 projMat = camContext.projMatrix;
	_float4x4 viewMat = camContext.viewMatrix;
	D3D11_VIEWPORT viewPort{};
	_uint numView = 1;
	engine->GetDeviceContext()->RSGetViewports(&numView, &viewPort);

	XMStoreFloat3(&mouse, XMVector3Unproject(XMVectorSetW(XMLoadFloat3(&mouse), 1.f),
		viewPort.TopLeftX, viewPort.TopLeftY, viewPort.Width, viewPort.Height,
		viewPort.MinDepth, viewPort.MaxDepth,
		XMLoadFloat4x4(&projMat),
		XMLoadFloat4x4(&viewMat),
		XMMatrixIdentity()));

	_float3 camPosition = camContext.camPosition;
	_float3 hoverPosition;
	XMStoreFloat3(&hoverPosition, XMPlaneIntersectLine(XMLoadFloat4(&m_PlaneXZ), XMLoadFloat3(&camPosition), XMLoadFloat3(&mouse)));

	return hoverPosition;
}

void Chunk::UpdatePlane()
{
	_float y = m_pTransform->GetPosition().y;

	m_PlaneXZ = _float4{ 0.f,1.f,0.f,-y };
}

void Chunk::UpdateParams()
{
	auto engine = EngineCore::GetInstance();
	auto mtrlInstance = GetComponent<SpriteComponent>()->GetMaterialInstance();

	_float3 position = m_pTransform->GetPosition();
	_float3 chunkMin{};
	_float cellSize{};
	_float3 hoverPosition{};

	chunkMin.x = position.x - CELL_SIZE * CHUNK_SIZE * 0.5f;
	chunkMin.y = 0.f;
	chunkMin.z = position.z - CELL_SIZE * CHUNK_SIZE * 0.5f;
	hoverPosition = GetHoverPosition();
	cellSize = CELL_SIZE;

	mtrlInstance->SetFloat3("g_HoverPosition", hoverPosition);
	mtrlInstance->SetFloat("g_CellSize", cellSize);
	mtrlInstance->SetFloat3("g_ChunkMin", chunkMin);
}
