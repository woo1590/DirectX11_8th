#include "pch.h"
#include "SpriteEffect.h"
#include "MaterialInstance.h"
#include "Material.h"
#include "Texture.h"
#include "EffectContainer.h"

//component
#include "SpriteComponent.h"

SpriteEffect::SpriteEffect()
	:EffectNode()
{
}

SpriteEffect::SpriteEffect(const SpriteEffect& prototype)
	:EffectNode(prototype),
	m_strMaterialTag(prototype.m_strMaterialTag),
	m_fSpeed(prototype.m_fSpeed),
	m_iMaxFrameIndex(prototype.m_iMaxFrameIndex),
	m_IsRepeat(prototype.m_IsRepeat),
	m_IsAnimated(prototype.m_IsAnimated),
	m_UseBillBoard(prototype.m_UseBillBoard),
	m_UseEaseOut(prototype.m_UseEaseOut),
	m_StartPosition(prototype.m_StartPosition),
	m_TargetPositon(prototype.m_TargetPositon),
	m_StartScale(prototype.m_StartScale),
	m_TargetScale(prototype.m_TargetScale),
	m_StartRotation(prototype.m_StartRotation),
	m_TargetRotation(prototype.m_TargetRotation),
	m_fStartAlpha(prototype.m_fStartAlpha),
	m_fTargetAlpha(prototype.m_fTargetAlpha),
	m_UVOffset(prototype.m_UVOffset),
	m_UVSpeed(prototype.m_UVSpeed)
{
}

SpriteEffect* SpriteEffect::Create(nlohmann::ordered_json prefab, const _string& filePath)
{
	SpriteEffect* Instance = new SpriteEffect();

	if (FAILED(Instance->Initialize_Prototype(prefab,filePath)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT SpriteEffect::Initialize_Prototype(nlohmann::ordered_json prefab, const _string& filePath)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	m_eRenderGroup = RenderGroup::Blend;

	namespace fs = std::filesystem;

	auto engine = EngineCore::GetInstance();
	
	m_strMaterialTag = prefab.at("material_name").get<_string>();
	auto material = engine->GetMaterial(ENUM_CLASS(LevelID::Static), m_strMaterialTag);

	if (!material)
	{
		material = Material::Create(engine->GetShader("Shader_VtxPoint"));
		fs::path dirPath = filePath;
		dirPath = dirPath.parent_path();

		_string textureName = prefab.at("texture_name").get<_string>();
		_uint numTexture = prefab.at("num_texture").get<_uint>();
		for (_uint i = 0; i < numTexture; ++i)
		{
			_string texturePath = dirPath.string() + "/" + textureName + std::to_string(i) + ".png";
			engine->LoadTextureFromFile(ENUM_CLASS(LevelID::Static), texturePath);
			material->SetTexture("g_DiffuseTexture", engine->GetTexture(ENUM_CLASS(LevelID::Static), texturePath));
		}
		engine->LoadMaterial(ENUM_CLASS(LevelID::Static), m_strMaterialTag, material);
	}

	_float3 position{};
	_float3 scale{};
	_float4 quaternion{};

	m_fDuration = prefab.at("duration").get<_float>();
	m_fSpeed = prefab.at("speed").get<_float>();
	m_iMaxFrameIndex = prefab.at("max_frames").get<_uint>();
	m_IsRepeat = prefab.at("repeat").get<_bool>();
	m_IsAnimated = prefab.at("animated").get<_bool>();

	position.x = prefab.at("position").at("x").get<_float>();
	position.y = prefab.at("position").at("y").get<_float>();
	position.z = prefab.at("position").at("z").get<_float>();

	scale.x = prefab.at("scale").at("x").get<_float>();
	scale.y = prefab.at("scale").at("y").get<_float>();
	scale.z = prefab.at("scale").at("z").get<_float>();

	quaternion.x = prefab.at("quaternion").at("x").get<_float>();
	quaternion.y = prefab.at("quaternion").at("y").get<_float>();
	quaternion.z = prefab.at("quaternion").at("z").get<_float>();
	quaternion.w = prefab.at("quaternion").at("w").get<_float>();

	m_UseBillBoard = prefab.at("use_billboard").get<_bool>();
	m_UseEaseOut = prefab.at("use_easeout").get<_bool>();

	m_StartPosition.x = prefab.at("start_position").at("x").get<_float>();
	m_StartPosition.y = prefab.at("start_position").at("y").get<_float>();
	m_StartPosition.z = prefab.at("start_position").at("z").get<_float>();

	m_TargetPositon.x = prefab.at("target_position").at("x").get<_float>();
	m_TargetPositon.y = prefab.at("target_position").at("y").get<_float>();
	m_TargetPositon.z = prefab.at("target_position").at("z").get<_float>();

	m_StartScale.x = prefab.at("start_scale").at("x").get<_float>();
	m_StartScale.y = prefab.at("start_scale").at("y").get<_float>();
	m_StartScale.z = prefab.at("start_scale").at("z").get<_float>();

	m_TargetScale.x = prefab.at("target_scale").at("x").get<_float>();
	m_TargetScale.y = prefab.at("target_scale").at("y").get<_float>();
	m_TargetScale.z = prefab.at("target_scale").at("z").get<_float>();

	m_StartRotation.x = prefab.at("start_rotation").at("x").get<_float>();
	m_StartRotation.y = prefab.at("start_rotation").at("y").get<_float>();
	m_StartRotation.z = prefab.at("start_rotation").at("z").get<_float>();

	m_TargetRotation.x = prefab.at("target_rotation").at("x").get<_float>();
	m_TargetRotation.y = prefab.at("target_rotation").at("y").get<_float>();
	m_TargetRotation.z = prefab.at("target_rotation").at("z").get<_float>();

	m_fStartAlpha = prefab.at("start_alpha").get<_float>();
	m_fTargetAlpha = prefab.at("target_alpha").get<_float>();

	m_UVSpeed.x = prefab.at("uv_speed").at("x").get<_float>();
	m_UVSpeed.y = prefab.at("uv_speed").at("y").get<_float>();

	m_pTransform->SetPosition(position);
	m_pTransform->SetScale(scale);
	m_pTransform->SetQuaternion(quaternion);

	return S_OK;
}

HRESULT SpriteEffect::Initialize(InitDESC* arg)
{
	PartObject::PART_OBJECT_DESC* desc = static_cast<PART_OBJECT_DESC*>(arg);
	m_pParent = desc->parent;	//여기서 addref하면 상호참조

	m_pTransform->SetParent(m_pParent->GetComponent<TransformComponent>());

	auto engine = EngineCore::GetInstance();

	/*sprite*/	
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.fSpeed = m_fSpeed;
	spriteDesc.iMaxFrameIndex = m_iMaxFrameIndex;
	spriteDesc.isRepeat = m_IsRepeat;
	spriteDesc.isAnimated = m_IsAnimated;

	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Point");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), m_strMaterialTag);

	auto mtrlInstance = sprite->GetMaterialInstance();
	if (!m_UseBillBoard)
		mtrlInstance->SetPass("NonBillBoard_Pass");
	mtrlInstance->SetFloat("g_CustomAlpha", 1.f);
	mtrlInstance->SetFloat2("g_UVOffset", _float2{ 0.f,0.f });

	return S_OK;
}

void SpriteEffect::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void SpriteEffect::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;
	if (m_fElapsedTime >= m_fDuration)
	{
		if (m_IsRepeat)
			m_fElapsedTime = 0.f;
		else
			m_IsActive = false;
	}
	else
	{
		_float t = m_fElapsedTime / m_fDuration;
		if (m_UseEaseOut)
			t = math::EaseOutQuint(t);

		_float3 currPosition{};
		_float3 currScale{};
		_float4 currRotation{};
		_float currAlpha{};

		_vector startQuaternion = XMQuaternionRotationRollPitchYaw(m_StartRotation.x, m_StartRotation.y, m_StartRotation.z);
		_vector targetQuaternion = XMQuaternionRotationRollPitchYaw(m_TargetRotation.x, m_TargetRotation.y, m_TargetRotation.z);

		XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&m_StartPosition), XMLoadFloat3(&m_TargetPositon), t));
		XMStoreFloat3(&currScale, XMVectorLerp(XMLoadFloat3(&m_StartScale), XMLoadFloat3(&m_TargetScale), t));
		XMStoreFloat4(&currRotation, XMQuaternionSlerp(startQuaternion, targetQuaternion, t));

		currAlpha = math::Lerp(m_fStartAlpha, m_fTargetAlpha, t);

		m_UVOffset.x += m_UVSpeed.x * dt;
		m_UVOffset.y += m_UVSpeed.y * dt;

		auto mtrlInstance = GetComponent<SpriteComponent>()->GetMaterialInstance();
		mtrlInstance->SetFloat("g_CustomAlpha", currAlpha);
		mtrlInstance->SetFloat2("g_UVOffset", m_UVOffset);

		m_pTransform->SetPosition(currPosition);
		m_pTransform->SetScale(currScale);
		m_pTransform->SetQuaternion(currRotation);
	}
}

void SpriteEffect::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT SpriteEffect::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	if (!m_IsActive)
		return S_OK;

	__super::ExtractRenderProxies(proxies);

	return S_OK;
}

Object* SpriteEffect::Clone(InitDESC* arg)
{
	SpriteEffect* Instance = new SpriteEffect(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void SpriteEffect::Free()
{
	__super::Free();
}
