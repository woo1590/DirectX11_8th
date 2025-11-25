#include "EnginePCH.h"
#include "ParticleSystemComponent.h"
#include "EngineCore.h"
#include "Random.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "VIBuffer_InstancingPoint.h"

#include "TransformComponent.h"

ParticleSystemComponent::ParticleSystemComponent(Object* owner)
	:Component(owner)
{
}

ParticleSystemComponent::ParticleSystemComponent(const ParticleSystemComponent& prototype)
	:Component(prototype)
{
}

ParticleSystemComponent* ParticleSystemComponent::Create(Object* object)
{
	ParticleSystemComponent* Instance = new ParticleSystemComponent(object);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ParticleSystemComponent::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT ParticleSystemComponent::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	PARTICLE_SYSTEM_DESC* desc = static_cast<PARTICLE_SYSTEM_DESC*>(arg);

	m_eSpace = desc->space;
	m_eDirMode = desc->dirMode;
	m_UseGravity = desc->useGravity;

	m_iNumBurst = desc->numBurst;
	m_fSpawnPerSec = desc->spawnPerSec;
	m_IsLoop = desc->isLoop;
	m_iMaxNumSpawnParticle = desc->maxSpawnParticle;

	m_SpawnAreaMin = desc->spawnAreaMin;
	m_SpawnAreaMax = desc->spawnAreaMax;

	m_VelocityMin = desc->velocityMin;
	m_VelocityMax = desc->velocityMax;

	m_SurfaceDir = desc->surfaceDir;
	m_fMinSpeed = desc->minSpeed;
	m_fMaxSpeed = desc->maxSpeed;
	m_fSpreadAngle = desc->spreadAngle;

	m_fUpSpeedMin = desc->upSpeedMin;
	m_fUpSpeedMax = desc->upSpeedMax;
	m_fSwaySpeed = desc->swaySpeed;

	m_SizeMin = desc->sizeMin;
	m_SizeMax = desc->sizeMax;
	m_fLifeMin = desc->lifeMin;
	m_fLifeMax = desc->lifeMax;

	if (!m_pMaterialInstance)
	{
		m_pMaterialInstance = MaterialInstance::Create();
		if (!m_pMaterialInstance)
			return E_FAIL;
	}

	return S_OK;
}

void ParticleSystemComponent::Update(_float dt)
{
	__super::Update(dt);

	SpawnParticles(dt);
	UpdateParticles(dt);
	BuildInstanceData();
	UploadInstanceData();
}

HRESULT ParticleSystemComponent::ExtractRenderProxy(TransformComponent* transform, std::vector<RenderProxy>& proxies)
{
	RenderProxy proxy{};
	proxy.buffer = m_pBuffer;
	proxy.material = m_pMaterial;
	proxy.materialInstance = m_pMaterialInstance;

	if (m_eSpace == ParticleSpace::Local)
		proxy.worldMatrix = transform->GetWorldMatrix();
	else
		XMStoreFloat4x4(&proxy.worldMatrix, XMMatrixIdentity());

	proxies.push_back(proxy);

	return S_OK;
}

void ParticleSystemComponent::SetPointParticle(_uint levelID, const _string& key)
{
	m_InstanceDatas.resize(g_iMaxParticle);
	m_Particles.resize(g_iMaxParticle);
	m_DeadParticleIndices.reserve(g_iMaxParticle);

	for (_uint i = 0; i < m_Particles.size(); ++i)
		m_DeadParticleIndices.push_back(i);

	m_pBuffer = VIBuffer_InstancingPoint::Create(levelID, key, g_iMaxParticle, m_InstanceDatas.data());
}

void ParticleSystemComponent::SetMaterial(_uint levelID, const _string& key)
{
	Safe_Release(m_pMaterial);

	m_pMaterial = EngineCore::GetInstance()->GetMaterial(levelID, key);
	m_pMaterial->AddRef();
}

void ParticleSystemComponent::Free()
{
	__super::Free();

	Safe_Release(m_pBuffer);
	Safe_Release(m_pMaterial);
	Safe_Release(m_pMaterialInstance);
}

#ifdef USE_IMGUI


void ParticleSystemComponent::RenderInspector()
{
	/*ImGui::PushID(this);

	if (!ImGui::CollapsingHeader("ParticleSystem",
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
	{
		ImGui::Separator();

		ImGui::Checkbox("Is Loop :", &m_IsLoop);
		ImGui::DragInt("Num Burst : ", reinterpret_cast<int*>(&m_iNumBurst));
		ImGui::DragFloat("Spawn Per Sec : ", &m_fSpawnPerSec);
		ImGui::DragFloat3("Spawn Area Min : ", &m_SpawnAreaMin.x);
		ImGui::DragFloat3("Spawn Area Max : ", &m_SpawnAreaMax.x);
		ImGui::DragFloat3("Velocity Area Min : ", &m_VelocityMin.x);
		ImGui::DragFloat3("Velocity Area Max : ", &m_VelocityMax.x);
		ImGui::DragFloat2("Size Min : ", &m_SizeMin.x);
		ImGui::DragFloat2("Size Max : ", &m_SizeMax.x);
		ImGui::DragFloat("Life Min : ", &m_fLifeMin);
		ImGui::DragFloat("Life Max : ", &m_fLifeMax);
	}

	ImGui::PopID();*/
}
#endif // USE_IMGUI

void ParticleSystemComponent::SpawnParticles(_float dt)
{
	if (m_iNumBurst > 0) //use burst
	{
		_uint burst = m_iNumBurst;
		m_iNumBurst = 0;

		for (_uint i = 0; i < burst; ++i)
		{
			if (m_DeadParticleIndices.empty())
				break;

			auto& particle = m_Particles[m_DeadParticleIndices.back()];
			m_DeadParticleIndices.pop_back();

			SetUpParticle(particle);
		}
	}
	else
	{
		if (!m_IsLoop && m_iNumSpawnParticle >= m_iMaxNumSpawnParticle)
			return;

		m_fSpawnAcc += m_fSpawnPerSec * dt;
		_uint spawnNum = static_cast<_uint>(m_fSpawnAcc);

		if (spawnNum)
		{
			m_fSpawnAcc -= spawnNum;
			m_iNumSpawnParticle += spawnNum;

			for (_uint i = 0; i < spawnNum; ++i)
			{
				if (m_DeadParticleIndices.empty())
					break;

				auto& particle = m_Particles[m_DeadParticleIndices.back()];
				m_DeadParticleIndices.pop_back();

				SetUpParticle(particle);
			}
		}
	}
}

void ParticleSystemComponent::UpdateParticles(_float dt)
{
	_uint index{};
	for (_uint i = 0; i < m_Particles.size(); ++i)
	{
		auto& particle = m_Particles[i];

		if (!particle.isAlive)
			continue;

		particle.lifeTime += dt;
		if (particle.lifeTime >= particle.maxLifeTime)
		{
			particle.isAlive = false;
			m_DeadParticleIndices.push_back(i);
		}

		if (m_eDirMode == ParticleDirMode::SwayUp)
		{
			_vector up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
			_vector worldRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
			_vector worldForward = XMVectorSet(0.f, 0.f, 1.f, 0.f);

			_float currAngle = particle.startPhase + m_fSwaySpeed * particle.lifeTime;
			_float cos = std::cosf(currAngle);
			_float sin = std::sinf(currAngle);

			_vector right = worldRight * cos - worldForward * sin;
			_vector forward = worldRight * sin + worldForward * cos;

			_vector dir = XMVector3Normalize(up + right * sin);
			XMStoreFloat3(&particle.velocity, dir * particle.upSpeed);
		}

		/*중력은 space world에서만 제대로 동작*/
		if (m_UseGravity)
			particle.velocity.y -= 200.f * dt;
		_vector velocity = XMLoadFloat3(&particle.velocity);
		_float3 currPosition = particle.position;
		_float3 nextPosition{};

		XMStoreFloat3(&nextPosition, XMLoadFloat3(&currPosition) + velocity * dt);
		particle.position = nextPosition;
	}
}

void ParticleSystemComponent::BuildInstanceData()
{
	m_iNumAliveParticle = 0;

	for (auto& particle : m_Particles)
	{
		if (!particle.isAlive)
			continue;

		auto& data = m_InstanceDatas[m_iNumAliveParticle++];

		data.position.x = particle.position.x;
		data.position.y = particle.position.y;
		data.position.z = particle.position.z;
		data.position.w = 1.f;

		data.velocity = particle.velocity;
		
		data.right = _float4(particle.size.x, 0.f, 0.f, 0.f);
		data.up = _float4(0.f, particle.size.y, 0.f, 0.f);
		data.forward = _float4(0.f, 0.f, 1.f, 0.f);

		data.lifeTime.x = particle.lifeTime;
		data.lifeTime.y = particle.maxLifeTime;
	}
}

void ParticleSystemComponent::UploadInstanceData()
{
	m_pBuffer->UploadInstanceData(m_InstanceDatas, m_iNumAliveParticle);
	D3D11_DEPTH_STENCIL_DESC;

}

void ParticleSystemComponent::SetUpParticle(PARTICLE& particle) const
{
	auto random = EngineCore::GetInstance()->GetRandom();

	particle.isAlive = true;

	particle.position.x = random->get<_float>(m_SpawnAreaMin.x, m_SpawnAreaMax.x);
	particle.position.y = random->get<_float>(m_SpawnAreaMin.y, m_SpawnAreaMax.y);
	particle.position.z = random->get<_float>(m_SpawnAreaMin.z, m_SpawnAreaMax.z);

	switch (m_eDirMode)
	{
	case Engine::ParticleDirMode::None:
	{
		particle.velocity.x = random->get<_float>(m_VelocityMin.x, m_VelocityMax.x);
		particle.velocity.y = random->get<_float>(m_VelocityMin.y, m_VelocityMax.y);
		particle.velocity.z = random->get<_float>(m_VelocityMin.z, m_VelocityMax.z);

	}break;
	case Engine::ParticleDirMode::SurfaceDir:
	{
		_vector dir = XMVector3Normalize(XMLoadFloat3(&m_SurfaceDir));
		_float speed = random->get<_float>(m_fMinSpeed, m_fMaxSpeed);

		if (m_fSpreadAngle > 0.f)
		{
			_vector worldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
			if (fabs(XMVectorGetY(dir)) > 0.99f)
				worldUp = XMVectorSet(0.f, 0.f, -1.f, 0.f);

			_vector right = XMVector3Normalize(XMVector3Cross(worldUp, dir));
			_vector up = XMVector3Normalize(XMVector3Cross(dir, right));

			_float randAngle0 = random->get<_float>(0.f, 1.f);
			_float randAngle1 = random->get<_float>(0.f, 1.f);

			_float cosMax = std::cos(m_fSpreadAngle);
			_float cosTheta = cosMax + (1.f - cosMax) * randAngle0;
			_float sinTheta = sqrtf((_float)max(0.f, 1.f - cosTheta * cosTheta));

			_float phi = math::TWO_PI * randAngle1;

			_vector offsetDir =
				cosTheta * dir +
				sinTheta * (std::cosf(phi) * right + std::sinf(phi) * up);

			dir = XMVector3Normalize(offsetDir);
		}

		XMStoreFloat3(&particle.velocity, dir * speed);

	}break;
	case ParticleDirMode::SwayUp:
	{
		particle.startPosition = particle.position;
		particle.startPhase = random->get<_float>(0.f, math::TWO_PI);
		particle.upSpeed = random->get<_float>(m_fUpSpeedMin, m_fUpSpeedMax);
	}break;
	default:
		break;
	}

	particle.size.x = random->get<_float>(m_SizeMin.x, m_SizeMax.x);
	particle.size.y = random->get<_float>(m_SizeMin.y, m_SizeMax.y);

	particle.lifeTime = 0.f;
	particle.maxLifeTime = random->get<_float>(m_fLifeMin, m_fLifeMax);
}
