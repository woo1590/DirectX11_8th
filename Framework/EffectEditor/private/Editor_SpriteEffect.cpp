#include "pch.h"
#include "Editor_SpriteEffect.h"
#include "Material.h"
#include "MaterialInstance.h"

//component
#include "SpriteComponent.h"

Editor_SpriteEffect::Editor_SpriteEffect()
	:Editor_EffectNode()
{
}

Editor_SpriteEffect::Editor_SpriteEffect(const Editor_SpriteEffect& prototype)
	:Editor_EffectNode(prototype)
{
}

Editor_SpriteEffect* Editor_SpriteEffect::Create()
{
	Editor_SpriteEffect* Instance = new Editor_SpriteEffect();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Editor_SpriteEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	m_strInstanceTag = "SpriteEffect";
	m_eRenderGroup = RenderGroup::Blend;

	return S_OK;
}

HRESULT Editor_SpriteEffect::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	auto sprite = GetComponent<SpriteComponent>();
	_string materialKey = "EditorMaterial" + std::to_string(m_iMaterialIndex);
	auto shader = engine->GetShader("Shader_VtxPoint");
	engine->LoadMaterial(ENUM_CLASS(LevelID::Static), materialKey, Material::Create(shader));

	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Point");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), materialKey);

	SpriteComponent::SPRITE_DESC desc{};
	sprite->Initialize(&desc);

	auto mtrlInstance = sprite->GetMaterialInstance();
	if (!m_UseBillboard)
		mtrlInstance->SetPass("NonBillBoard_Pass");
	mtrlInstance->SetFloat("g_CustomAlpha", 1.f);
	mtrlInstance->SetFloat2("g_UVOffset", _float2{ 0.f,0.f });

	m_pSpriteMaterial = engine->GetMaterial(ENUM_CLASS(LevelID::Static), materialKey);
	m_pSpriteMaterial->AddRef();

	++m_iMaterialIndex;

	return S_OK;
}

void Editor_SpriteEffect::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Editor_SpriteEffect::Update(_float dt)
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

		_vector startQuaternion = XMQuaternionRotationRollPitchYaw(m_StartRotaion.x, m_StartRotaion.y, m_StartRotaion.z);
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

void Editor_SpriteEffect::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT Editor_SpriteEffect::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	if (!m_IsActive)
		return S_OK;

	__super::ExtractRenderProxies(proxies);

	return S_OK;
}

void Editor_SpriteEffect::SpriteEffectSetUp()
{
	_bool isDirty = false;

	auto sprite = GetComponent<SpriteComponent>();

	ImGui::Text(m_strMaterialTag.c_str());
	if (ImGui::Button("Rename"))
		ImGui::OpenPopup("Rename##clip");

	if (ImGui::BeginPopupModal("Rename##clip", nullptr,
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
	{
		static char rename[128];

		if (ImGui::IsWindowAppearing())
		{
			strncpy_s(rename, 128, m_strMaterialTag.c_str(), 128);
			ImGui::SetKeyboardFocusHere();
		}

		ImGui::TextUnformatted("Input new clip name..");
		if (ImGui::InputText("##name", rename, 128,
			ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_strMaterialTag = rename;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}
	
	isDirty |= ImGui::Checkbox("Use BillBoard : ", &m_UseBillboard);
	ImGui::Checkbox("Use EaseOut : ", &m_UseEaseOut);
	ImGui::DragFloat("Durtaion : ", &m_fDuration);
	ImGui::DragFloat("Start Alpha : ", &m_fStartAlpha);
	ImGui::DragFloat("Target Alpha : ", &m_fTargetAlpha);
	isDirty |= ImGui::DragFloat2("UV Speed : ", &m_UVSpeed.x);
	ImGui::DragFloat3("Start Position : ", &m_StartPosition.x);
	ImGui::DragFloat3("Target Position : ", &m_TargetPositon.x);
	ImGui::DragFloat3("Start Scale : ", &m_StartScale.x);
	ImGui::DragFloat3("Target Scale : ", &m_TargetScale.x);
	ImGui::DragFloat3("Start Rotation : ", &m_StartRotaion.x);
	ImGui::DragFloat3("Target Rotation : ", &m_TargetRotation.x);

	isDirty |= ImGui::DragFloat("Speed : ", &m_fSpeed);
	isDirty |= ImGui::DragInt("MaxFrameIndex : ", &m_iMaxFrameIndex);
	isDirty |= ImGui::Checkbox("Is Repeat : ", &m_IsRepeat);
	isDirty |= ImGui::Checkbox("Is Animated : ", &m_IsAnimated);

	if (isDirty)
	{
		m_UVOffset = _float2{ 0.f,0.f };

		SpriteComponent::SPRITE_DESC desc{};
		desc.fSpeed = m_fSpeed;
		desc.iMaxFrameIndex = m_iMaxFrameIndex;
		desc.isRepeat = m_IsRepeat;
		desc.isAnimated = m_IsAnimated;

		sprite->Initialize(&desc);

		if (m_UseBillboard)
			sprite->GetMaterialInstance()->SetPass("Default_Pass");
		else
			sprite->GetMaterialInstance()->SetPass("NonBillBoard_Pass");
	}

	if (ImGui::Button("Add Sprite Texture"))
		AddCurrTexture();
	if (ImGui::Button("Replace Material"))
		ReplaceMaterial();
}

void Editor_SpriteEffect::AddCurrTexture()
{
	if (!m_pContext->textures.empty())
	{
		for (_uint i = 0; i < m_pContext->textures.size(); ++i)
		{
			m_SpriteTextureNames.push_back(m_pContext->textureTags[i]);
			m_pSpriteMaterial->SetTexture("g_DiffuseTexture", m_pContext->textures[i]);
		}
	}
}

void Editor_SpriteEffect::ReplaceMaterial()
{
	Safe_Release(m_pSpriteMaterial);

	auto engine = EngineCore::GetInstance();

	_string materialKey = "EditorMaterial" + std::to_string(m_iMaterialIndex);
	auto shader = engine->GetShader("Shader_VtxPoint");
	engine->LoadMaterial(ENUM_CLASS(LevelID::Static), materialKey, Material::Create(shader));

	auto sprite = GetComponent<SpriteComponent>();
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), materialKey);

	m_pSpriteMaterial = engine->GetMaterial(ENUM_CLASS(LevelID::Static), materialKey);
	m_pSpriteMaterial->AddRef();

	++m_iMaterialIndex;
}

_string Editor_SpriteEffect::SeperateString(_string name)
{
	_uint dotIndex = name.find_last_of('.');
	name = name.substr(0, dotIndex);

	_uint end = static_cast<_uint>(name.size() - 1);
	while (end >= 0 && std::isdigit(static_cast<unsigned char>(name[end])))
		--end;

	_uint numStart = end + 1;

	name = name.substr(0, numStart);

	return name;
}

void Editor_SpriteEffect::RenderInspector()
{
	ImGui::PushID(this);
	SpriteEffectSetUp();
	ImGui::PopID();

	__super::RenderInspector();
}

void Editor_SpriteEffect::Start()
{
	auto sprite = GetComponent<SpriteComponent>();
	sprite->SetFrameIndex(0);

	m_fElapsedTime = 0.f;
	m_IsActive = true;
}

void Editor_SpriteEffect::Import(nlohmann::ordered_json& j)
{
	using namespace nlohmann;

	_string textureName = j.at("texture_name").get<_string>();
	_uint numTexture = j.at("num_texture").get<_uint>();

	auto engine = EngineCore::GetInstance();

	for (_uint i = 0; i < numTexture; ++i)
	{
		_string textureFullName = textureName + std::to_string(i);
		
		auto iter = m_pContext->pAllTextures->find(textureFullName);
		if (iter != m_pContext->pAllTextures->end())
		{
			m_SpriteTextureNames.push_back(iter->first);
			m_pSpriteMaterial->SetTexture("g_DiffuseTexture", iter->second);
		}
		else
			MSG_BOX("Wrong texture name");
	}

	_float3 position{}, scale{};
	_float4 quaternion{};

	m_strMaterialTag = j.at("material_name").get<_string>();

	m_fDuration = j.at("duration").get<_float>();
	m_fSpeed = j.at("speed").get<_float>();
	m_iMaxFrameIndex = j.at("max_frames").get<_uint>();
	m_IsRepeat = j.at("repeat").get<_bool>();
	m_IsAnimated = j.at("animated").get<_bool>();

	position.x = j.at("position").at("x").get<_float>();
	position.y = j.at("position").at("y").get<_float>();
	position.z = j.at("position").at("z").get<_float>();

	scale.x = j.at("scale").at("x").get<_float>();
	scale.y = j.at("scale").at("y").get<_float>();
	scale.z = j.at("scale").at("z").get<_float>();

	quaternion.x = j.at("quaternion").at("x").get<_float>();
	quaternion.y = j.at("quaternion").at("y").get<_float>();
	quaternion.z = j.at("quaternion").at("z").get<_float>();
	quaternion.w = j.at("quaternion").at("w").get<_float>();

	m_UseEaseOut = j.at("use_easeout").get<_float>();

	m_StartPosition.x = j.at("start_position").at("x").get<_float>();
	m_StartPosition.y = j.at("start_position").at("y").get<_float>();
	m_StartPosition.z = j.at("start_position").at("z").get<_float>();

	m_TargetPositon.x = j.at("target_position").at("x").get<_float>();
	m_TargetPositon.y = j.at("target_position").at("y").get<_float>();
	m_TargetPositon.z = j.at("target_position").at("z").get<_float>();

	m_StartScale.x = j.at("start_scale").at("x").get<_float>();
	m_StartScale.y = j.at("start_scale").at("y").get<_float>();
	m_StartScale.z = j.at("start_scale").at("z").get<_float>();

	m_TargetScale.x = j.at("target_scale").at("x").get<_float>();
	m_TargetScale.y = j.at("target_scale").at("y").get<_float>();
	m_TargetScale.z = j.at("target_scale").at("z").get<_float>();

	m_StartRotaion.x = j.at("start_rotation").at("x").get<_float>();
	m_StartRotaion.y = j.at("start_rotation").at("y").get<_float>();
	m_StartRotaion.z = j.at("start_rotation").at("z").get<_float>();

	m_TargetRotation.x = j.at("target_rotation").at("x").get<_float>();
	m_TargetRotation.y = j.at("target_rotation").at("y").get<_float>();
	m_TargetRotation.z = j.at("target_rotation").at("z").get<_float>();

	m_fStartAlpha = j.at("start_alpha").get<_float>();
	m_fTargetAlpha = j.at("target_alpha").get<_float>();

	m_UVSpeed.x = j.at("uv_speed").at("x").get<_float>();
	m_UVSpeed.y = j.at("uv_speed").at("y").get<_float>();
}

void Editor_SpriteEffect::Export(nlohmann::ordered_json& j)
{
	using namespace nlohmann;

	_string textureName = SeperateString(m_SpriteTextureNames.front());

	_float3 position = m_pTransform->GetPosition();
	_float3 scale = m_pTransform->GetScale();
	_float4 quaternion = m_pTransform->GetQuaternion();

	ordered_json node = ordered_json::object();
	node["type"] = "sprite";

	node["material_name"] = m_strMaterialTag;
	node["texture_name"] = textureName;
	node["num_texture"] = m_SpriteTextureNames.size();

	node["duration"] = m_fDuration;
	node["speed"] = m_fSpeed;
	node["max_frames"] = m_iMaxFrameIndex;
	node["repeat"] = m_IsRepeat;
	node["animated"] = m_IsAnimated;
	node["position"] = { {"x",position.x},{"y",position.y},{"z",position.z} };
	node["scale"] = { {"x",scale.x},{"y",scale.y},{"z",scale.z} };
	node["quaternion"] = { {"x",quaternion.x},{"y",quaternion.y},{"z",quaternion.z},{"w",quaternion.w} };

	node["use_easeout"] = m_UseEaseOut;

	node["start_position"] = { {"x",m_StartPosition.x},{"y",m_StartPosition.y},{"z",m_StartPosition.z} };
	node["target_position"] = { {"x",m_TargetPositon.x},{"y",m_TargetPositon.y},{"z",m_TargetPositon.z} };

	node["start_scale"] = { {"x",m_StartScale.x},{"y",m_StartScale.y},{"z",m_StartScale.z} };
	node["target_scale"] = { {"x",m_TargetScale.x},{"y",m_TargetScale.y},{"z",m_TargetScale.z} };

	node["start_rotation"] = { {"x",m_StartRotaion.x},{"y",m_StartRotaion.y},{"z",m_StartRotaion.z} };
	node["target_rotation"] = { {"x",m_TargetRotation.x},{"y",m_TargetRotation.y},{"z",m_TargetRotation.z} };

	node["start_alpha"] = m_fStartAlpha;
	node["target_alpha"] = m_fTargetAlpha;

	node["uv_speed"] = { {"x",m_UVSpeed.x},{"y",m_UVSpeed.y} };

	j.push_back(std::move(node));
}

Object* Editor_SpriteEffect::Clone(InitDESC* arg)
{
	Editor_SpriteEffect* Instance = new Editor_SpriteEffect(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Editor_SpriteEffect::Free()
{
	__super::Free();
	
	Safe_Release(m_pSpriteMaterial);
}
