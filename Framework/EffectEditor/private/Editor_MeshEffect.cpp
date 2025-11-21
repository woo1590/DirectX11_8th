#include "pch.h"
#include "Editor_MeshEffect.h"
#include "MaterialInstance.h"

#include "ModelComponent.h"

Editor_MeshEffect::Editor_MeshEffect()
	:Editor_EffectNode()
{
}

Editor_MeshEffect::Editor_MeshEffect(const Editor_MeshEffect& prototype)
	:Editor_EffectNode(prototype)
{
}

Editor_MeshEffect* Editor_MeshEffect::Create()
{
	Editor_MeshEffect* Instance = new Editor_MeshEffect();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Editor_MeshEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	m_eRenderGroup = RenderGroup::Blend;

	return S_OK;
}

HRESULT Editor_MeshEffect::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto model = GetComponent<ModelComponent>();
	model->Initialize(nullptr);

	return S_OK;
}

void Editor_MeshEffect::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Editor_MeshEffect::Update(_float dt)
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
					--m_iCurrFrame;
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
		_float3 currScale{};
		_float t = m_fElapsedTime / m_fDuration;
		t = std::clamp(t, 0.f, 1.f);

		if (m_UseEaseOut)
			t = math::EaseOutQuint(t);

		XMStoreFloat3(&currScale, XMVectorLerp(XMLoadFloat3(&m_StartScale), XMLoadFloat3(&m_TargetScale), t));
		m_pTransform->SetScale(currScale);
	}
}

void Editor_MeshEffect::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Editor_MeshEffect::RenderInspector()
{
	ImGui::PushID(this);
	MeshEffectSetUp();
	ImGui::PopID();

	__super::RenderInspector();
}

void Editor_MeshEffect::Start()
{
	if (0 != m_iMaxFrame)
		m_iCurrFrame = 0;
	else
		m_UVOffset = _float2{ 0.f,0.f };

	m_fElapsedTime = 0.f;
}

void Editor_MeshEffect::Import(nlohmann::ordered_json& j)
{
	using namespace nlohmann;

	auto engine = EngineCore::GetInstance();
}

void Editor_MeshEffect::Export(nlohmann::ordered_json& j)
{
	using namespace nlohmann;

	_float3 position = m_pTransform->GetPosition();
	_float3 scale = m_pTransform->GetScale();
	_float4 quaternion = m_pTransform->GetQuaternion();

	ordered_json node = ordered_json::object();
	node["type"] = "mesh";

	node["model_tag"] = m_strModelTag;
	node["pass_tag"] = m_strPassTag;

	node["position"] = { {"x",position.x},{"y",position.y},{"z",position.z} };
	node["scale"] = { {"x",scale.x},{"y",scale.y},{"z",scale.z} };
	node["quaternion"] = { {"x",quaternion.x},{"y",quaternion.y},{"z",quaternion.z},{"w",quaternion.w} };

	node["duration"] = m_fDuration;
	node["speed"] = m_fSpeed;
	node["max_frames"] = m_iMaxFrame;
	node["repeat"] = m_IsRepeat;

	node["uv_offset"] = { {"x",m_UVOffset.x},{"y",m_UVOffset.y} };
	node["uv_speed"] = { {"x",m_UVSpeed.x},{"y",m_UVSpeed.y} };

	node["use_easeout"] = m_UseEaseOut;
	node["start_scale"] = { {"x",m_StartScale.x},{"y",m_StartScale.y},{"z",m_StartScale.z} };
	node["target_scale"] = { {"x",m_TargetScale.x},{"y",m_TargetScale.y},{"z",m_TargetScale.z} };

	j.push_back(std::move(node));
}

Object* Editor_MeshEffect::Clone(InitDESC* arg)
{
	Editor_MeshEffect* Instance = new Editor_MeshEffect(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Editor_MeshEffect::Free()
{
	__super::Free();
}

void Editor_MeshEffect::MeshEffectSetUp()
{
	_bool isDirty = false;
	SetCurrModel();

	if (ImGui::Button("UV Anim Mode"))
	{
		m_strPassTag = "UVAnim_Pass";
		isDirty = true;
	}

	if (ImGui::Button("Sprite Anim Mode"))
	{
		m_strPassTag = "SpriteAnim_Pass";
		isDirty = true;
	}

	if (isDirty)
		GetComponent<ModelComponent>()->GetMaterialInstance()->SetPass(m_strPassTag);

	if ("UVAnim_Pass" == m_strPassTag)
	{
		ImGui::DragFloat2("UV Speed", &m_UVSpeed.x);
	}
	else if("SpriteAnim_Pass" == m_strPassTag)
	{
		ImGui::Checkbox("Is Repeat", &m_IsRepeat);
		ImGui::DragFloat("Speed", &m_fSpeed);
		ImGui::DragInt("Max Frame", reinterpret_cast<_int*>(&m_iMaxFrame));
	}
		
	ImGui::Checkbox("Use EaseOut", &m_UseEaseOut);
	ImGui::DragFloat("Duration", &m_fDuration);
	ImGui::DragFloat3("Start Scale", &m_StartScale.x);
	ImGui::DragFloat3("Target Scale", &m_TargetScale.x);
}

void Editor_MeshEffect::SetCurrModel()
{
	if (-1 != m_pContext->selectModelIndex)
	{
		if (ImGui::Button("Set Selected Model"))
		{
			_string modelTag = m_pContext->modelTags[m_pContext->selectModelIndex];
			
			auto engine = EngineCore::GetInstance();
			auto model = engine->GetModel(ENUM_CLASS(LevelID::Static), modelTag);

			if (!model)
			{
				MSG_BOX("Model is not exist");
				return;
			}

			auto modelCom = GetComponent<ModelComponent>()->SetModel(ENUM_CLASS(LevelID::Static), modelTag);
		}
	}
}
