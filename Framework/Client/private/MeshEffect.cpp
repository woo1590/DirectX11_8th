#include "pch.h"
#include "MeshEffect.h"
#include "MaterialInstance.h"
#include "ModelComponent.h"
#include "EffectContainer.h"

MeshEffect::MeshEffect()
	:EffectNode()
{
}

MeshEffect::MeshEffect(const MeshEffect& prototype)
	:EffectNode(prototype),
	m_strModelTag(prototype.m_strModelTag),
	m_strPassTag(prototype.m_strPassTag),
	m_IsRepeat(prototype.m_IsRepeat),
	m_iMaxFrame(prototype.m_iMaxFrame),
	m_fSpeed(prototype.m_fSpeed),
	m_UVOffset(prototype.m_UVOffset),
	m_UVSpeed(prototype.m_UVSpeed),
	m_UseEaseOut(prototype.m_UseEaseOut),
	m_StartScale(prototype.m_StartScale),
	m_TargetScale(prototype.m_TargetScale)
{
}

MeshEffect* MeshEffect::Create(nlohmann::ordered_json prefab, const _string& filePath)
{
	MeshEffect* Instance = new MeshEffect();

	if (FAILED(Instance->Initialize_Prototype(prefab,filePath)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT MeshEffect::Initialize_Prototype(nlohmann::ordered_json prefab, const _string& filePath)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	m_eRenderGroup = RenderGroup::Blend;

	_float3 position{};
	_float3 scale{};
	_float4 quaternion{};

	m_strModelTag = prefab.at("model_tag").get<_string>();
	m_strPassTag = prefab.at("pass_tag").get<_string>();

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

	m_fDuration = prefab.at("duration").get<_float>();
	m_fSpeed = prefab.at("speed").get<_float>();
	m_iMaxFrame = prefab.at("max_frames").get<_uint>();
	m_IsRepeat = prefab.at("repeat").get<_bool>();

	m_UVOffset.x = prefab.at("uv_offset").at("x").get<_float>();
	m_UVOffset.y = prefab.at("uv_offset").at("y").get<_float>();

	m_UVSpeed.x = prefab.at("uv_speed").at("x").get<_float>();
	m_UVSpeed.y = prefab.at("uv_speed").at("y").get<_float>();

	m_UseEaseOut = prefab.at("use_easeout").get<_bool>();

	m_StartScale.x = prefab.at("start_scale").at("x").get<_float>();
	m_StartScale.y = prefab.at("start_scale").at("y").get<_float>();
	m_StartScale.z = prefab.at("start_scale").at("z").get<_float>();

	m_TargetScale.x = prefab.at("target_scale").at("x").get<_float>();
	m_TargetScale.y = prefab.at("target_scale").at("y").get<_float>();
	m_TargetScale.z = prefab.at("target_scale").at("z").get<_float>();

	return S_OK;
}
	
HRESULT MeshEffect::Initialize(InitDESC* arg)
{
	/*if (FAILED(__super::Initialize(arg)))
		return E_FAIL;*/

	PartObject::PART_OBJECT_DESC* desc = static_cast<PART_OBJECT_DESC*>(arg);
	m_pParent = desc->parent;

	m_pTransform->SetParent(m_pParent->GetComponent<TransformComponent>());

	auto engine = EngineCore::GetInstance();

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->Initialize(nullptr);
	model->SetModel(ENUM_CLASS(LevelID::Static), m_strModelTag);

	auto mtrlInstance = model->GetMaterialInstance();
	mtrlInstance->SetPass(m_strPassTag);

	return S_OK;
}

void MeshEffect::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void MeshEffect::Update(_float dt)
{
	__super::Update(dt);

	auto mtrlInstance = GetComponent<ModelComponent>()->GetMaterialInstance();

	/*sprite anim*/
	if (0 != m_iMaxFrame)
	{
		m_fSpriteElapsedTime += dt * m_fSpeed;
		if (m_fSpriteElapsedTime >= 1.f)
		{
			++m_iCurrFrame;
			if (m_iCurrFrame >= m_iMaxFrame)
			{
				if (m_IsRepeat)
					m_iCurrFrame = 0;
				else
				{
					m_IsActive = false;
					--m_iCurrFrame;
				}
			}

			m_fSpriteElapsedTime -= 1.f;
		}

		mtrlInstance->SetFloat("g_UVRatio", 1.f / m_iMaxFrame);
		mtrlInstance->SetInt("g_FrameIndex", m_iCurrFrame);
	}
	/*uv anim*/
	else
	{
		m_UVOffset.x += dt * m_UVSpeed.x;
		m_UVOffset.y += dt * m_UVSpeed.y;

		mtrlInstance->SetFloat2("g_UVOffset", m_UVOffset);
	}

	if (!m_IsRepeat)
	{
		m_fElapsedTime += dt;

		if (m_fElapsedTime >= m_fDuration)
		{
			m_IsActive = false;
			return;
		}

		_float3 currScale{};
		_float t = m_fElapsedTime / m_fDuration;
		t = std::clamp(t, 0.f, 1.f);

		if (m_UseEaseOut)
			t = math::EaseOutQuint(t);

		XMStoreFloat3(&currScale, XMVectorLerp(XMLoadFloat3(&m_StartScale), XMLoadFloat3(&m_TargetScale), t));
		m_pTransform->SetScale(currScale);
	}
}

void MeshEffect::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT MeshEffect::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	if (!m_IsActive)
		return S_OK;

	__super::ExtractRenderProxies(proxies);

	return S_OK;
}

Object* MeshEffect::Clone(InitDESC* arg)
{
	MeshEffect* Instance = new MeshEffect(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void MeshEffect::Free()
{
	__super::Free();
}
