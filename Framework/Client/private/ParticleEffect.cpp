#include "pch.h"
#include "ParticleEffect.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "EffectContainer.h"

#include "ParticleSystemComponent.h"

ParticleEffect::ParticleEffect()
	:EffectNode()
{
}

ParticleEffect::ParticleEffect(const ParticleEffect& prototype)
	:EffectNode(prototype),
	m_strMaterialTag(prototype.m_strMaterialTag),
	m_eSpace(prototype.m_eSpace),
	m_eDirMode(prototype.m_eDirMode),
	m_UseGravity(prototype.m_UseGravity),
	m_iNumBurst(prototype.m_iNumBurst),
	m_fSpawnPerSec(prototype.m_fSpawnPerSec),
	m_fSpawnAcc(prototype.m_fSpawnAcc),
	m_IsLoop(prototype.m_IsLoop),
	m_iMaxNumSpawnParticle(prototype.m_iMaxNumSpawnParticle),
	m_SpawnAreaMin(prototype.m_SpawnAreaMin),
	m_SpawnAreaMax(prototype.m_SpawnAreaMax),
	m_VelocityMin(prototype.m_VelocityMin),
	m_VelocityMax(prototype.m_VelocityMax),
	m_SurfaceDir(prototype.m_SurfaceDir),
	m_fMinSpeed(prototype.m_fMinSpeed),
	m_fMaxSpeed(prototype.m_fMaxSpeed),
	m_fSpreadAngle(prototype.m_fSpreadAngle),
	m_fUpSpeedMin(prototype.m_fUpSpeedMin),
	m_fUpSpeedMax(prototype.m_fUpSpeedMax),
	m_fSwaySpeed(prototype.m_fSwaySpeed),
	m_SizeMin(prototype.m_SizeMin),
	m_SizeMax(prototype.m_SizeMax),
	m_fLifeMin(prototype.m_fLifeMin),
	m_fLifeMax(prototype.m_fLifeMax)
{
}

ParticleEffect* ParticleEffect::Create(nlohmann::ordered_json prefab, const _string& filePath)
{
	ParticleEffect* Instance = new ParticleEffect();

	if (FAILED(Instance->Initialize_Prototype(prefab, filePath)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ParticleEffect::Initialize_Prototype(nlohmann::ordered_json prefab, const _string& filePath)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ParticleSystemComponent>();
	m_eRenderGroup = RenderGroup::NonLight;

	namespace fs = std::filesystem;

	auto engine = EngineCore::GetInstance();
	m_strMaterialTag = prefab.at("material_name").get<_string>();
	auto material = engine->GetMaterial(ENUM_CLASS(LevelID::Static), m_strMaterialTag);

	if (!material)
	{
		material = Material::Create(engine->GetShader("Shader_VtxInstancePoint"));
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

	_float3 position{}, scale{};
	_float4 quaternion{};

	m_fDuration = prefab.at("duration").get<_float>();

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

	_string space = prefab.at("space").get<_string>();
	_string dirMode = prefab.at("dir_mode").get<_string>();
	if ("local" == space)
		m_eSpace = ParticleSpace::Local;
	else
		m_eSpace = ParticleSpace::World;

	if ("none" == dirMode)
		m_eDirMode = ParticleDirMode::None;
	else if ("surface_dir" == dirMode)
		m_eDirMode = ParticleDirMode::SurfaceDir;
	else
		m_eDirMode = ParticleDirMode::SwayUp;

	m_UseGravity = prefab.at("use_gravity").get<_bool>();
	m_iNumBurst = prefab.at("num_burst").get<_uint>();
	m_fSpawnPerSec = prefab.at("spawn_per_sec").get<_float>();
	m_fSpawnAcc = 0.f;

	m_IsLoop = prefab.at("is_loop").get<_bool>();
	m_iMaxNumSpawnParticle = prefab.at("max_num_spawn_particle").get<_uint>();

	m_SpawnAreaMin.x = prefab.at("spawn_area_min").at("x").get<_float>();
	m_SpawnAreaMin.y = prefab.at("spawn_area_min").at("y").get<_float>();
	m_SpawnAreaMin.z = prefab.at("spawn_area_min").at("z").get<_float>();

	m_SpawnAreaMax.x = prefab.at("spawn_area_max").at("x").get<_float>();
	m_SpawnAreaMax.y = prefab.at("spawn_area_max").at("y").get<_float>();
	m_SpawnAreaMax.z = prefab.at("spawn_area_max").at("z").get<_float>();

	m_VelocityMin.x = prefab.at("velocity_min").at("x").get<_float>();
	m_VelocityMin.y = prefab.at("velocity_min").at("y").get<_float>();
	m_VelocityMin.z = prefab.at("velocity_min").at("z").get<_float>();

	m_VelocityMax.x = prefab.at("velocity_max").at("x").get<_float>();
	m_VelocityMax.y = prefab.at("velocity_max").at("y").get<_float>();
	m_VelocityMax.z = prefab.at("velocity_max").at("z").get<_float>();

	m_fMinSpeed = prefab.at("speed_min").get<_float>();
	m_fMaxSpeed = prefab.at("speed_max").get<_float>();
	m_fSpreadAngle = prefab.at("spread_angle").get<_float>();

	m_fUpSpeedMin = prefab.at("up_speed_min").get<_float>();
	m_fUpSpeedMax = prefab.at("up_speed_max").get<_float>();
	m_fSwaySpeed = prefab.at("sway_speed").get<_float>();

	m_SizeMin.x = prefab.at("size_min").at("x").get<_float>();
	m_SizeMin.y = prefab.at("size_min").at("y").get<_float>();
	m_SizeMax.x = prefab.at("size_max").at("x").get<_float>();
	m_SizeMax.y = prefab.at("size_max").at("y").get<_float>();

	m_fLifeMin = prefab.at("life_min").get<_float>();
	m_fLifeMax = prefab.at("life_max").get<_float>();

	return S_OK;
}

HRESULT ParticleEffect::Initialize(InitDESC* arg)
{
	PartObject::PART_OBJECT_DESC* desc = static_cast<PART_OBJECT_DESC*>(arg);
	m_pParent = desc->parent;	//여기서 addref하면 상호참조

	m_pTransform->SetParent(m_pParent->GetComponent<TransformComponent>());

	auto engine = EngineCore::GetInstance();

	/*particle system*/	
	ParticleSystemComponent::PARTICLE_SYSTEM_DESC particleDesc{};
	particleDesc.space = m_eSpace;
	particleDesc.dirMode = m_eDirMode;
	particleDesc.useGravity = m_UseGravity;

	particleDesc.isLoop = m_IsLoop;
	particleDesc.maxSpawnParticle = m_iMaxNumSpawnParticle;
	particleDesc.numBurst = m_iNumBurst;
	particleDesc.spawnPerSec = m_fSpawnPerSec;
	particleDesc.spawnAreaMin = m_SpawnAreaMin;
	particleDesc.spawnAreaMax = m_SpawnAreaMax;

	particleDesc.velocityMin = m_VelocityMin;
	particleDesc.velocityMax = m_VelocityMax;
	particleDesc.surfaceDir = m_SurfaceDir;
	particleDesc.minSpeed = m_fMinSpeed;
	particleDesc.maxSpeed = m_fMaxSpeed;
	particleDesc.spreadAngle = math::ToRadian(m_fSpreadAngle);

	particleDesc.upSpeedMin = m_fUpSpeedMin;
	particleDesc.upSpeedMax = m_fUpSpeedMax;
	particleDesc.swaySpeed = m_fSwaySpeed;

	particleDesc.sizeMin = m_SizeMin;
	particleDesc.sizeMax = m_SizeMax;
	particleDesc.lifeMin = m_fLifeMin;
	particleDesc.lifeMax = m_fLifeMax;

	auto particle = GetComponent<ParticleSystemComponent>();
	particle->Initialize(&particleDesc);

	particle->SetPointParticle(ENUM_CLASS(LevelID::Static), "Buffer_Point");
	particle->SetMaterial(ENUM_CLASS(LevelID::Static), m_strMaterialTag);

	return S_OK;
}

HRESULT ParticleEffect::LateInitialize()
{
	auto particle = GetComponent<ParticleSystemComponent>();

	if (m_eSpace == ParticleSpace::World)
	{
		_float3 position = m_pParent->GetComponent<TransformComponent>()->GetPosition();
		_float3 minPosition{}, maxPosition{};
		XMStoreFloat3(&minPosition, XMLoadFloat3(&position) + XMLoadFloat3(&m_SpawnAreaMin));
		XMStoreFloat3(&maxPosition, XMLoadFloat3(&position) + XMLoadFloat3(&m_SpawnAreaMax));

		particle->SetSpawnArea(minPosition, maxPosition);
	}

	if (m_eDirMode == ParticleDirMode::SurfaceDir)
	{
		particle->SetSurfaceDir(static_cast<EffectContainer*>(m_pParent)->GetSurfaceDir());
	}

	return S_OK;
}

void ParticleEffect::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void ParticleEffect::Update(_float dt)
{
	if (!m_IsLateInitialized)
		return;

	__super::Update(dt);

	m_fElapsedTime += dt;
	if (m_fElapsedTime >= m_fDuration && !m_IsLoop)
		m_IsActive = false;
}

void ParticleEffect::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT ParticleEffect::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	if (!m_IsActive)
		return S_OK;

	__super::ExtractRenderProxies(proxies);

	return S_OK;
}

Object* ParticleEffect::Clone(InitDESC* arg)
{
	ParticleEffect* Instance = new ParticleEffect(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;	
}

void ParticleEffect::Free()
{
	__super::Free();
}
