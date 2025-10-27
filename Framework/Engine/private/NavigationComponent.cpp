#include "EnginePCH.h"
#include "NavigationComponent.h"
#include "NavigationSystem.h"
#include "Object.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"

NavigationComponent::NavigationComponent(Object* owner)
	:Component(owner)
{
}

NavigationComponent::NavigationComponent(const NavigationComponent& prototype)
	:Component(prototype)
{
}

NavigationComponent* NavigationComponent::Create(Object* owner)
{
	NavigationComponent* Instance = new NavigationComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT NavigationComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT NavigationComponent::Initialize(InitDESC* arg)
{
	return S_OK;
}

void NavigationComponent::AttachTransform()
{
	m_pTransform = m_pOwner->GetComponent<TransformComponent>();
	m_pTransform->AddRef();
}

void NavigationComponent::AttachRigidBody()
{
	m_pRigidBody = m_pOwner->GetComponent<RigidBodyComponent>();
	m_pRigidBody->AddRef();
}

void NavigationComponent::AttachSystem(NavigationSystem* system)
{
	m_pNavigationSystem = system;
	m_pNavigationSystem->AddRef();
}

void NavigationComponent::SpawnInCell(_uint cellIndex)
{
	m_iCurrCellIndex = cellIndex;

	if (!m_pNavigationSystem->IsCellExist(m_iCurrCellIndex))
		m_iCurrCellIndex = 0;

	_float3 position = m_pNavigationSystem->GetPositionInCell(m_iCurrCellIndex);
	position.y = m_pNavigationSystem->GetHeight(position, m_iCurrCellIndex);

	m_pTransform->SetPosition(position);
}

void NavigationComponent::MoveTo(_float3 deltaPosition)
{
	if (!m_pTransform)
		return;

	_float3 currPosition = m_pTransform->GetPosition();
	_float3 nextPosition;
	XMStoreFloat3(&nextPosition, XMLoadFloat3(&currPosition) + XMLoadFloat3(&deltaPosition));

	if (IsMove(nextPosition))
		m_pTransform->SetPosition(nextPosition);
	else
		return;
}

void NavigationComponent::MoveByVelocity(_float dt)
{
	if (!m_pTransform || !m_pRigidBody)
		return;

	_float3 currPosition = m_pTransform->GetPosition();
	_float3 velocity = m_pRigidBody->GetVelocity();
	_float3 nextPosition;
	XMStoreFloat3(&nextPosition, XMLoadFloat3(&currPosition) + XMLoadFloat3(&velocity) * dt);

	if (m_IsInLink)
	{
		if (IsMove(nextPosition))
		{
			_float y = GetHeight(nextPosition);

			if (m_pRigidBody->IsGround())
			{
				nextPosition.y = y;
				m_pTransform->SetPosition(nextPosition);
			}
			else
			{
				if (nextPosition.y <= y)
				{
					m_pRigidBody->SetGround(true);
					m_IsInLink = false;
					m_iCurrLinkedCellIndex = -1;
					nextPosition.y = y;

					m_pTransform->SetPosition(nextPosition);
				}
				else
					m_pTransform->SetPosition(nextPosition);
			}
		}
		else
		{
			if (m_iCurrCellIndex == m_iCurrLinkedCellIndex)
				m_pTransform->SetPosition(nextPosition);
			else
			{
				nextPosition = MakeSlideVector(currPosition, nextPosition);

				if (IsMove(nextPosition))
				{
					_float y = GetHeight(nextPosition);

					if (m_pRigidBody->IsGround())
					{
						nextPosition.y = y;
						m_pTransform->SetPosition(nextPosition);
					}
					else
					{
						if (nextPosition.y <= y)
						{
							m_pRigidBody->SetGround(true);
							nextPosition.y = y;

							m_pTransform->SetPosition(nextPosition);
						}
						else
							m_pTransform->SetPosition(nextPosition);
					}
				}
			}
		}
	}
	else
	{
		if (IsMove(nextPosition))
		{
			_float y = GetHeight(nextPosition);

			if (m_pRigidBody->IsGround())
			{
				nextPosition.y = y;
				m_pTransform->SetPosition(nextPosition);
			}
			else
			{
				if (nextPosition.y <= y)
				{
					m_pRigidBody->SetGround(true);
					nextPosition.y = y;

					m_pTransform->SetPosition(nextPosition);
				}
				else
					m_pTransform->SetPosition(nextPosition);
			}
		}
		else
		{
			if (IsLinkedCell(nextPosition))
			{
				m_pRigidBody->SetGround(false);
				m_pTransform->SetPosition(nextPosition);
				m_iCurrLinkedCellIndex = m_iCurrCellIndex;
				m_IsInLink = true;
			}
			else
			{
				nextPosition = MakeSlideVector(currPosition, nextPosition);

				if (IsMove(nextPosition))
				{
					_float y = GetHeight(nextPosition);

					if (m_pRigidBody->IsGround())
					{
						nextPosition.y = y;
						m_pTransform->SetPosition(nextPosition);
					}
					else
					{
						if (nextPosition.y <= y)
						{
							m_pRigidBody->SetGround(true);
							nextPosition.y = y;

							m_pTransform->SetPosition(nextPosition);
						}
						else
							m_pTransform->SetPosition(nextPosition);
					}
				}
			}
		}
	}
}

_bool NavigationComponent::IsLinkedCell(_float3 position)
{
	return m_pNavigationSystem->IsLinkedCell(position, m_iCurrCellIndex);
}

_bool NavigationComponent::IsMove(_float3 position)
{
	return m_pNavigationSystem->IsMove(position, m_iCurrCellIndex);
}

_float NavigationComponent::GetHeight(_float3 position)
{
	return m_pNavigationSystem->GetHeight(position, m_iCurrCellIndex);
}

_float3 NavigationComponent::MakeSlideVector(_float3 position, _float3 nextPosition)
{
	return m_pNavigationSystem->MakeSlideVector(position, nextPosition, m_iCurrCellIndex);
}

void NavigationComponent::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationSystem);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRigidBody);
}

#ifdef USE_IMGUI
void NavigationComponent::RenderInspector()
{
	ImGui::PushID(this);

	if (!ImGui::CollapsingHeader("Navigation",
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
	{
		ImGui::Separator();
		ImGui::Text("Current Cell : %d", m_iCurrCellIndex);

	}

	ImGui::PopID();
}
#endif