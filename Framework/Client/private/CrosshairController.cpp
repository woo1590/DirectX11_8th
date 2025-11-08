#include "pch.h"
#include "CrosshairController.h"
#include "EngineCore.h"
#include "Crosshair.h"

CrosshairController::CrosshairController()
{
}

CrosshairController* CrosshairController::Create()
{
	CrosshairController* Instance = new CrosshairController();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT CrosshairController::Initialize()
{
	m_Crosshairs.resize(ENUM_CLASS(CrosshairDir::Count));
	m_fMinSpread = 20.f;
	m_fMaxSpread = 40.f;
	auto engine = EngineCore::GetInstance();

	/*add up*/
	{
		Object* crosshairUp = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Crosshair", nullptr);
		auto upTransform = crosshairUp->GetComponent<TransformComponent>();
		upTransform->Rotate(_float3{ 0.f,0.f,math::ToRadian(-45.f) });
		upTransform->SetPosition(_float3{ 0.f,m_fMinSpread,0.f });

		m_Crosshairs[ENUM_CLASS(CrosshairDir::Up)] = crosshairUp;
	}

	/*add down*/
	{
		Object* crosshairDown = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Crosshair", nullptr);
		auto downTransform = crosshairDown->GetComponent<TransformComponent>();
		downTransform->Rotate(_float3{ 0.f,0.f,math::ToRadian(135.f) });
		downTransform->SetPosition(_float3{ 0.f,-m_fMinSpread,0.f });

		m_Crosshairs[ENUM_CLASS(CrosshairDir::Down)] = crosshairDown;
	}

	/*add right*/
	{
		Object* crosshairRight = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Crosshair", nullptr);
		auto rightTransform = crosshairRight->GetComponent<TransformComponent>();
		rightTransform->Rotate(_float3{ 0.f,0.f,math::ToRadian(-135.f) });
		rightTransform->SetPosition(_float3{ m_fMinSpread,0.f,0.f });

		m_Crosshairs[ENUM_CLASS(CrosshairDir::Right)] = crosshairRight;
	}

	/*add left*/
	{
		Object* crosshairLeft = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Crosshair", nullptr);
		auto leftTransform = crosshairLeft->GetComponent<TransformComponent>();
		leftTransform->Rotate(_float3{ 0.f,0.f,math::ToRadian(45.f) });
		leftTransform->SetPosition(_float3{ -m_fMinSpread,0.f,0.f });

		m_Crosshairs[ENUM_CLASS(CrosshairDir::Left)] = crosshairLeft;
	}

	return S_OK;
}

void CrosshairController::Update(_float dt)
{
	dt*=m_fLerpScale;
	if (m_IsSpread)
	{
		/*up*/
		{
			auto transform = m_Crosshairs[ENUM_CLASS(CrosshairDir::Up)]->GetComponent<TransformComponent>();
			_float3 currPosition = transform->GetPosition();
			_float3 targetPosition{ 0.f,m_fMaxSpread,0.f };
			XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&currPosition), XMLoadFloat3(&targetPosition), dt));

			transform->SetPosition(currPosition);
		}
		
		/*down*/
		{

			auto transform = m_Crosshairs[ENUM_CLASS(CrosshairDir::Down)]->GetComponent<TransformComponent>();
			_float3 currPosition = transform->GetPosition();
			_float3 targetPosition{ 0.f,-m_fMaxSpread,0.f };
			XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&currPosition), XMLoadFloat3(&targetPosition), dt));

			transform->SetPosition(currPosition);
		}
		
		/*right*/
		{

			auto transform = m_Crosshairs[ENUM_CLASS(CrosshairDir::Right)]->GetComponent<TransformComponent>();
			_float3 currPosition = transform->GetPosition();
			_float3 targetPosition{ m_fMaxSpread,0.f,0.f };
			XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&currPosition), XMLoadFloat3(&targetPosition), dt));

			transform->SetPosition(currPosition);
		}
		
		/*left*/
		{

			auto transform = m_Crosshairs[ENUM_CLASS(CrosshairDir::Left)]->GetComponent<TransformComponent>();
			_float3 currPosition = transform->GetPosition();
			_float3 targetPosition{ -m_fMaxSpread,0.f,0.f };
			XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&currPosition), XMLoadFloat3(&targetPosition), dt));

			transform->SetPosition(currPosition);
		}
	}
	else
	{
		/*up*/
		{
			auto transform = m_Crosshairs[ENUM_CLASS(CrosshairDir::Up)]->GetComponent<TransformComponent>();
			_float3 currPosition = transform->GetPosition();
			_float3 targetPosition{ 0.f,m_fMinSpread,0.f };
			XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&currPosition), XMLoadFloat3(&targetPosition), dt));

			transform->SetPosition(currPosition);
		}

		/*down*/
		{

			auto transform = m_Crosshairs[ENUM_CLASS(CrosshairDir::Down)]->GetComponent<TransformComponent>();
			_float3 currPosition = transform->GetPosition();
			_float3 targetPosition{ 0.f,-m_fMinSpread,0.f };
			XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&currPosition), XMLoadFloat3(&targetPosition), dt));

			transform->SetPosition(currPosition);
		}

		/*right*/
		{

			auto transform = m_Crosshairs[ENUM_CLASS(CrosshairDir::Right)]->GetComponent<TransformComponent>();
			_float3 currPosition = transform->GetPosition();
			_float3 targetPosition{ m_fMinSpread,0.f,0.f };
			XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&currPosition), XMLoadFloat3(&targetPosition), dt));

			transform->SetPosition(currPosition);
		}

		/*left*/
		{

			auto transform = m_Crosshairs[ENUM_CLASS(CrosshairDir::Left)]->GetComponent<TransformComponent>();
			_float3 currPosition = transform->GetPosition();
			_float3 targetPosition{ -m_fMinSpread,0.f,0.f };
			XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&currPosition), XMLoadFloat3(&targetPosition), dt));

			transform->SetPosition(currPosition);
		}
	}
}

HRESULT CrosshairController::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	for (const auto& crossHair : m_Crosshairs)
		crossHair->ExtractRenderProxies(proxies);

	return S_OK;
}

void CrosshairController::Free()
{
	__super::Free();

	for (auto& cross : m_Crosshairs)
		Safe_Release(cross);
	m_Crosshairs.clear();
}
