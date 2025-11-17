#include "pch.h"
#include "Editor_ParticleEffect.h"
#include "Material.h"
#include "MaterialInstance.h"

#include "ParticleSystemComponent.h"

Editor_ParticleEffect::Editor_ParticleEffect()
	:Editor_EffectNode()
{
}

Editor_ParticleEffect::Editor_ParticleEffect(const Editor_ParticleEffect& prototype)
	:Editor_EffectNode(prototype)
{
}

Editor_ParticleEffect* Editor_ParticleEffect::Create()
{
	Editor_ParticleEffect* Instance = new Editor_ParticleEffect();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Editor_ParticleEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ParticleSystemComponent>();

	return S_OK;
}

HRESULT Editor_ParticleEffect::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	_string materialKey = "EditorMaterial" + std::to_string(m_iMaterialIndex);
	auto shader = engine->GetShader("Shader_VtxInstancePoint");
	engine->LoadMaterial(ENUM_CLASS(LevelID::Static), materialKey, Material::Create(shader));

	auto particle = GetComponent<ParticleSystemComponent>();
	particle->SetPointParticle(ENUM_CLASS(LevelID::Static),"Buffer_Point");
	particle->SetMaterial(ENUM_CLASS(LevelID::Static), materialKey);

	ParticleSystemComponent::PARTICLE_SYSTEM_DESC desc{};
	particle->Initialize(&desc);

	m_pParticleMaterial = engine->GetMaterial(ENUM_CLASS(LevelID::Static), materialKey);
	m_pParticleMaterial->AddRef();

	++m_iMaterialIndex;

	return S_OK;
}

void Editor_ParticleEffect::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Editor_ParticleEffect::Update(_float dt)
{
	__super::Update(dt);
}

void Editor_ParticleEffect::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT Editor_ParticleEffect::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	auto particle = GetComponent<ParticleSystemComponent>();

	particle->ExtractRenderProxy(m_pTransform, proxies[ENUM_CLASS(RenderGroup::Blend)]);

	return S_OK;
}

void Editor_ParticleEffect::RenderInspector()
{
	ImGui::PushID(this);
	ParticleEffectSetUp();
	ImGui::PopID();

	__super::RenderInspector();
}

void Editor_ParticleEffect::Start()
{
	ParticleSystemComponent::PARTICLE_SYSTEM_DESC desc{};

	desc.space = m_eSpace;
	desc.dirMode = m_eDirMode;
	desc.useGravity = m_UseGravity;

	desc.isLoop = m_IsLoop;
	desc.numBurst = m_iNumBurst;
	desc.spawnPerSec = m_fSpawnPerSec;
	desc.spawnAreaMin = m_SpawnAreaMin;
	desc.spawnAreaMax = m_SpawnAreaMax;

	desc.velocityMin = m_VelocityMin;
	desc.velocityMax = m_VelocityMax;
	desc.surfaceDir = m_SurfaceDir;
	desc.minSpeed = m_fMinSpeed;
	desc.maxSpeed = m_fMaxSpeed;
	desc.spreadAngle = math::ToRadian(m_fSpreadAngle);

	desc.upSpeedMin = m_fUpSpeedMin;
	desc.upSpeedMax = m_fUpSpeedMax;
	desc.swaySpeed = m_fSwaySpeed;

	desc.sizeMin = m_SizeMin;
	desc.sizeMax = m_SizeMax;
	desc.lifeMin = m_fLifeMin;
	desc.lifeMax = m_fLifeMax;

	auto particle = GetComponent<ParticleSystemComponent>();
	particle->Initialize(&desc);
}

void Editor_ParticleEffect::Import(nlohmann::ordered_json& j)
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
			m_ParticleTextureNames.push_back(iter->first);
			m_pParticleMaterial->SetTexture("g_DiffuseTexture", iter->second);
		}
		else
			MSG_BOX("Wrong texture name");
	}

	_float3 position{}, scale{};
	_float4 quaternion{};

	m_strMaterialTag = j.at("material_name").get<_string>();

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

	m_fDuration = j.at("duration").get<_float>();
	m_UseGravity = j.at("use_gravity").get<_bool>();
	m_iNumBurst = j.at("num_burst").get<_uint>();
	m_fSpawnPerSec = j.at("spawn_per_sec").get<_float>();
	m_fSpawnAcc = 0.f;

	m_IsLoop = j.at("is_loop").get<_bool>();
	m_iMaxNumSpawnParticle = j.at("max_num_spawn_particle").get<_uint>();
	m_SpawnAreaMin.x = j.at("spawn_area_min").at("x").get<_float>();
	m_SpawnAreaMin.y = j.at("spawn_area_min").at("y").get<_float>();
	m_SpawnAreaMin.z = j.at("spawn_area_min").at("z").get<_float>();

	m_SpawnAreaMax.x = j.at("spawn_area_max").at("x").get<_float>();
	m_SpawnAreaMax.y = j.at("spawn_area_max").at("y").get<_float>();
	m_SpawnAreaMax.z = j.at("spawn_area_max").at("z").get<_float>();

	m_VelocityMin.x = j.at("velocity_min").at("x").get<_float>();
	m_VelocityMin.y = j.at("velocity_min").at("y").get<_float>();
	m_VelocityMin.z = j.at("velocity_min").at("z").get<_float>();

	m_VelocityMax.x = j.at("velocity_max").at("x").get<_float>();
	m_VelocityMax.y = j.at("velocity_max").at("y").get<_float>();
	m_VelocityMax.z = j.at("velocity_max").at("z").get<_float>();

	m_fMinSpeed = j.at("speed_min").get<_float>();
	m_fMaxSpeed = j.at("speed_max").get<_float>();
	m_fSpreadAngle = j.at("spread_angle").get<_float>();

	m_fUpSpeedMin = j.at("up_speed_min").get<_float>();
	m_fUpSpeedMax = j.at("up_speed_max").get<_float>();
	m_fSwaySpeed = j.at("sway_speed").get<_float>();

	m_SizeMin.x = j.at("size_min").at("x").get<_float>();
	m_SizeMin.y = j.at("size_min").at("y").get<_float>();
	m_SizeMax.x = j.at("size_max").at("x").get<_float>();
	m_SizeMax.y = j.at("size_max").at("y").get<_float>();

	m_fLifeMin = j.at("life_min").get<_float>();
	m_fLifeMax = j.at("life_max").get<_float>();
}

void Editor_ParticleEffect::Export(nlohmann::ordered_json& j)
{
	using namespace nlohmann;

	_string textureName = SeperateString(m_ParticleTextureNames.front());

	_float3 position = m_pTransform->GetPosition();
	_float3 scale = m_pTransform->GetScale();
	_float4 quaternion = m_pTransform->GetQuaternion();

	ordered_json node = ordered_json::object();
	node["type"] = "particle";

	node["material_name"] = m_strMaterialTag;
	node["texture_name"] = textureName;
	node["num_texture"] = m_ParticleTextureNames.size();
	node["position"] = { {"x",position.x},{"y",position.y},{"z",position.z} };
	node["scale"] = { {"x",scale.x},{"y",scale.y},{"z",scale.z} };
	node["quaternion"] = { {"x",quaternion.x},{"y",quaternion.y},{"z",quaternion.z},{"w",quaternion.w} };

	switch (m_eSpace)
	{
	case Engine::ParticleSpace::Local:
	{
		node["space"] = "local";
	}break;
	case Engine::ParticleSpace::World:
	{
		node["space"] = "world";
	}break;
	default:
		break;
	}

	switch (m_eDirMode)
	{
	case Engine::ParticleDirMode::None:
	{
		node["dir_mode"] = "none";
	}break;
	case Engine::ParticleDirMode::SurfaceDir:
	{
		node["dir_mode"] = "surface_dir";
	}break;
	case Engine::ParticleDirMode::SwayUp:
	{
		node["dir_mode"] = "sway_up";
	}break;
	default:
		break;
	}
	
	node["use_gravity"] = m_UseGravity;
	node["duration"] = m_fDuration;
	node["num_burst"] = m_iNumBurst;
	node["spawn_per_sec"] = m_fSpawnPerSec;
	node["spawn_acc"] = m_fSpawnAcc;

	node["is_loop"] = m_IsLoop;
	node["max_num_spawn_particle"] = m_iMaxNumSpawnParticle;
	node["spawn_area_min"] = { {"x",m_SpawnAreaMin.x},{"y",m_SpawnAreaMin.y},{"z",m_SpawnAreaMin.z} };
	node["spawn_area_max"] = { {"x",m_SpawnAreaMax.x},{"y",m_SpawnAreaMax.y},{"z",m_SpawnAreaMax.z} };

	node["velocity_min"] = { {"x",m_VelocityMin.x},{"y",m_VelocityMin.y},{"z",m_VelocityMin.z} };
	node["velocity_max"] = { {"x",m_VelocityMax.x},{"y",m_VelocityMax.y},{"z",m_VelocityMax.z} };

	node["speed_min"] = m_fMinSpeed;
	node["speed_max"] = m_fMaxSpeed;
	node["spread_angle"] = m_fSpreadAngle;

	node["up_speed_min"] = m_fUpSpeedMin;
	node["up_speed_max"] = m_fUpSpeedMax;
	node["sway_speed"] = m_fSwaySpeed;

	node["size_min"] = { {"x",m_SizeMin.x},{"y",m_SizeMin.y} };
	node["size_max"] = { {"x",m_SizeMax.x}, {"y",m_SizeMax.y} };
	node["life_min"] = m_fLifeMin;
	node["life_max"] = m_fLifeMax;

	j.push_back(node);
}

Object* Editor_ParticleEffect::Clone(InitDESC* arg)
{
	Editor_ParticleEffect* Instance = new Editor_ParticleEffect(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Editor_ParticleEffect::Free()
{
	__super::Free();

	Safe_Release(m_pParticleMaterial);
}

void Editor_ParticleEffect::ParticleEffectSetUp()
{
	_bool isDirty = false;

	if (!ImGui::CollapsingHeader("Particle Params",
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
	{
		ImGui::Separator();

		m_eSpace == ParticleSpace::Local ? ImGui::Text("Space : Local") : ImGui::Text("Space : World");
		m_eDirMode == ParticleDirMode::None ? ImGui::Text("Dir Mode : None") : ImGui::Text("Dir Mode : SurfaceDir");

		if (ImGui::Button("Change Space"))
		{
			isDirty = true;
			m_eSpace = m_eSpace == ParticleSpace::Local ? ParticleSpace::World : ParticleSpace::Local;
		}
		if (ImGui::Button("Dir Mode : None"))
		{
			isDirty = true;
			m_eDirMode = ParticleDirMode::None;
		}
		if (ImGui::Button("Dir Mode : SurfaceDir"))
		{
			isDirty = true;
			m_eDirMode = ParticleDirMode::SurfaceDir;
		}
		if (ImGui::Button("Dir Mode : SwayUp"))
		{
			isDirty = true;
			m_eDirMode = ParticleDirMode::SwayUp;
		}

		isDirty |= ImGui::Checkbox("Use Gravity", &m_UseGravity);
		isDirty |= ImGui::Checkbox("Is Loop :", &m_IsLoop);
		isDirty |= ImGui::DragInt("Num Burst : ", reinterpret_cast<int*>(&m_iNumBurst));
		isDirty |= ImGui::DragFloat("Spawn Per Sec : ", &m_fSpawnPerSec);
		isDirty |= ImGui::DragFloat3("Spawn Area Min : ", &m_SpawnAreaMin.x);
		isDirty |= ImGui::DragFloat3("Spawn Area Max : ", &m_SpawnAreaMax.x);

		ImGui::SeparatorText("Dir mode : None");
		isDirty |= ImGui::DragFloat3("Velocity Area Min : ", &m_VelocityMin.x);
		isDirty |= ImGui::DragFloat3("Velocity Area Max : ", &m_VelocityMax.x);

		ImGui::SeparatorText("Dir Mode : Surface Dir");
		isDirty |= ImGui::DragFloat3("Surface Dir : ", &m_SurfaceDir.x);
		isDirty |= ImGui::DragFloat("Speed Min : ", &m_fMinSpeed);
		isDirty |= ImGui::DragFloat("Speed Max : ", &m_fMaxSpeed);
		isDirty |= ImGui::DragFloat("Spread Angle : ", &m_fSpreadAngle);

		ImGui::SeparatorText("Dir Mode : Surface Dir");
		isDirty |= ImGui::DragFloat("Up Speed Min : ", &m_fUpSpeedMin);
		isDirty |= ImGui::DragFloat("Up Speed Max : ", &m_fUpSpeedMax);
		isDirty |= ImGui::DragFloat("Sway Speed : ", &m_fSwaySpeed);

		isDirty |= ImGui::DragFloat2("Size Min : ", &m_SizeMin.x);
		isDirty |= ImGui::DragFloat2("Size Max : ", &m_SizeMax.x);
		isDirty |= ImGui::DragFloat("Life Min : ", &m_fLifeMin);
		isDirty |= ImGui::DragFloat("Life Max : ", &m_fLifeMax);

		if (isDirty &&
			m_SpawnAreaMax.x >= m_SpawnAreaMin.x && m_SpawnAreaMax.y >= m_SpawnAreaMin.y && m_SpawnAreaMax.z >= m_SpawnAreaMin.z &&
			m_VelocityMax.x >= m_VelocityMin.x && m_VelocityMax.y >= m_VelocityMin.y && m_VelocityMax.z >= m_VelocityMin.z &&
			m_fMaxSpeed >= m_fMinSpeed &&
			m_fUpSpeedMax >= m_fUpSpeedMin &&
			m_SizeMax.x >= m_SizeMin.x && m_SizeMax.y >= m_SizeMin.y &&
			m_fLifeMax >= m_fLifeMin)
		{
			ParticleSystemComponent::PARTICLE_SYSTEM_DESC desc{};

			desc.space = m_eSpace;
			desc.dirMode = m_eDirMode;
			desc.useGravity = m_UseGravity;
			desc.numBurst = m_iNumBurst;
			desc.spawnPerSec = m_fSpawnPerSec;

			desc.isLoop = m_IsLoop;
			desc.maxSpawnParticle = m_iMaxNumSpawnParticle;

			desc.spawnAreaMin = m_SpawnAreaMin;
			desc.spawnAreaMax = m_SpawnAreaMax;

			desc.velocityMin = m_VelocityMin;
			desc.velocityMax = m_VelocityMax;
			desc.surfaceDir = m_SurfaceDir;
			desc.minSpeed = m_fMinSpeed;
			desc.maxSpeed = m_fMaxSpeed;
			desc.spreadAngle = math::ToRadian(m_fSpreadAngle);

			desc.upSpeedMin = m_fUpSpeedMin;
			desc.upSpeedMax = m_fUpSpeedMax;
			desc.swaySpeed = m_fSwaySpeed;

			desc.sizeMin = m_SizeMin;
			desc.sizeMax = m_SizeMax;
			desc.lifeMin = m_fLifeMin;
			desc.lifeMax = m_fLifeMax;

			auto particle = GetComponent<ParticleSystemComponent>();
			particle->Initialize(&desc);
		}

		if (ImGui::Button("Add Sprite Texture"))
			AddCurrTexture();
		if (ImGui::Button("Replace Material"))
			ReplaceMaterial();
	}
}

void Editor_ParticleEffect::AddCurrTexture()
{
	if (!m_pContext->textures.empty())
	{
		for (_uint i = 0; i < m_pContext->textures.size(); ++i)
		{
			m_ParticleTextureNames.push_back(m_pContext->textureTags[i]);
			m_pParticleMaterial->SetTexture("g_DiffuseTexture", m_pContext->textures[i]);
		}
	}
}

void Editor_ParticleEffect::ReplaceMaterial()
{
	Safe_Release(m_pParticleMaterial);

	auto engine = EngineCore::GetInstance();

	_string materialKey = "EditorMaterial" + std::to_string(m_iMaterialIndex);
	auto shader = engine->GetShader("Shader_VtxInstancePoint");
	engine->LoadMaterial(ENUM_CLASS(LevelID::Static), materialKey, Material::Create(shader));

	auto particle = GetComponent<ParticleSystemComponent>();
	particle->SetMaterial(ENUM_CLASS(LevelID::Static), materialKey);

	m_pParticleMaterial = engine->GetMaterial(ENUM_CLASS(LevelID::Static), materialKey);
	m_pParticleMaterial->AddRef();

	++m_iMaterialIndex;
}

_string Editor_ParticleEffect::SeperateString(_string name)
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
