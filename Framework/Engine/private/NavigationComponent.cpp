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

void NavigationComponent::SetCurrCellIndex(_uint cellIndex)
{
	m_iCurrCellIndex = cellIndex;

	if (!m_pNavigationSystem->IsCellExist(m_iCurrCellIndex))
		m_iCurrCellIndex = 0;
}

void NavigationComponent::FindPath(_float3 startPosition, _uint startCellIndex, _float3 targetPosition, _uint targetCellIndex)
{
	m_CurrPath = m_pNavigationSystem->FindPath(startPosition, startCellIndex, targetPosition, targetCellIndex);
	m_iCurrPathCursor = 0;
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
	{
		nextPosition = MakeSlideVector(currPosition, nextPosition);

		if (IsMove(nextPosition))
		{
			m_pTransform->SetPosition(nextPosition);
		}
	}
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

void NavigationComponent::MoveByPath(_float dt)
{
	if (m_CurrPath.isAvailable)
	{
		if (m_iCurrCellIndex == m_CurrPath.cellIndices.back())
		{
			_float3 currPosition = m_pTransform->GetPosition();
			_float3 targetPosition = m_CurrPath.targetPosition;
			currPosition.y = 0.f;
			targetPosition.y = 0.f;

			_float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&targetPosition) - XMLoadFloat3(&currPosition)));

			if (distance < m_fArriveRange)
				m_CurrPath.isAvailable = false;
			else
			{
				_float3 currDir = m_pTransform->GetForward();
				_float3 targetDir{};
				_float3 velocity{};
				_float3 nextPosition{};

				XMStoreFloat3(&targetDir, XMVector3Normalize(XMLoadFloat3(&targetPosition) - XMLoadFloat3(&currPosition)));
				XMStoreFloat3(&targetDir, XMVectorLerp(XMLoadFloat3(&currDir), XMLoadFloat3(&targetDir), dt * 8.f));
				XMStoreFloat3(&velocity, m_fMoveSpeed * XMLoadFloat3(&targetDir));
				XMStoreFloat3(&nextPosition, XMLoadFloat3(&currPosition) + XMLoadFloat3(&velocity) * dt);

				if (IsMove(nextPosition))
				{
					_float y = GetHeight(nextPosition);
					nextPosition.y = y;

					m_pTransform->SetPosition(nextPosition);
				}
				else
				{
					nextPosition = MakeSlideVector(currPosition, nextPosition);

					if (IsMove(nextPosition))
					{
						_float y = GetHeight(nextPosition);
						nextPosition.y = y;
						m_pTransform->SetPosition(nextPosition);
					}
				}

				m_pTransform->SetForward(targetDir);
			}

			return;
		}

		if (m_iCurrPathCursor >= m_CurrPath.cellIndices.size() - 1)
		{
			_float3 currPosition = m_pTransform->GetPosition();
			_float3 targetPosition{};
			_float3 nextPosition{};

			targetPosition = m_CurrPath.targetPosition;
			currPosition.y = 0.f;
			targetPosition.y = 0.f;

			_float3 currDir = m_pTransform->GetForward();
			_float3 targetDir{};
			_float3 velocity{};
			XMStoreFloat3(&targetDir, XMVector3Normalize(XMLoadFloat3(&targetPosition) - XMLoadFloat3(&currPosition)));
			XMStoreFloat3(&targetDir, XMVectorLerp(XMLoadFloat3(&currDir), XMLoadFloat3(&targetDir), dt * 8.f));
			XMStoreFloat3(&velocity, m_fMoveSpeed * XMLoadFloat3(&targetDir));
			XMStoreFloat3(&nextPosition, XMLoadFloat3(&currPosition) + XMLoadFloat3(&velocity) * dt);

			if (IsMove(nextPosition))
			{
				_float y = GetHeight(nextPosition);
				nextPosition.y = y;
				m_pTransform->SetPosition(nextPosition);
			}
			else
			{
				nextPosition = MakeSlideVector(currPosition, nextPosition);

				if (IsMove(nextPosition))
				{
					_float y = GetHeight(nextPosition);
					nextPosition.y = y;

					m_pTransform->SetPosition(nextPosition);
					m_pTransform->SetPosition(nextPosition);
				}
			}

			m_pTransform->SetForward(targetDir);

			return;
		}

		if (m_iCurrCellIndex != m_CurrPath.cellIndices[m_iCurrPathCursor])
		{
			_float3 currPosition = m_pTransform->GetPosition();
			currPosition.y = 0.f;

			_float3 targetPosition{};
			_float3 nextPosition{};

			_float3 portalLeftPoint = m_CurrPath.portals[m_iCurrPathCursor].leftPoint;
			_float3 portalRightPoint = m_CurrPath.portals[m_iCurrPathCursor].rightPoint;
			portalLeftPoint.y = 0.f;
			portalRightPoint.y = 0.f;

			_vector portalDir = XMVector3Normalize(XMLoadFloat3(&portalRightPoint) - XMLoadFloat3(&portalLeftPoint));
			_float3 A{}, B{};

			XMStoreFloat3(&A, XMLoadFloat3(&portalLeftPoint) + portalDir * 15.f);
			XMStoreFloat3(&B, XMLoadFloat3(&portalRightPoint) - portalDir * 15.f);

			_vector AB = XMLoadFloat3(&A) - XMLoadFloat3(&B);
			_vector AQ = XMLoadFloat3(&currPosition) - XMLoadFloat3(&A);
			_float lengthSqr = XMVectorGetX(XMVector3Dot(AB, AB));
			if (lengthSqr < math::ELIPSON)
			{
				++m_iCurrPathCursor;
				return;
			}

			_float t = XMVectorGetX(XMVector3Dot(AQ, AB)) / lengthSqr;
			t = std::clamp(t, 0.f, 1.f);
			XMStoreFloat3(&targetPosition, XMLoadFloat3(&A) + t * AB);
			targetPosition.y = 0.f;

			_float3 currDir = m_pTransform->GetForward();
			_float3 targetDir{};
			_float3 velocity{};
			XMStoreFloat3(&targetDir, XMVector3Normalize(XMLoadFloat3(&targetPosition) - XMLoadFloat3(&currPosition)));
			XMStoreFloat3(&targetDir, XMVectorLerp(XMLoadFloat3(&currDir), XMLoadFloat3(&targetDir), dt * 8.f));
			XMStoreFloat3(&velocity, m_fMoveSpeed * XMLoadFloat3(&targetDir));
			XMStoreFloat3(&nextPosition, XMLoadFloat3(&currPosition) + XMLoadFloat3(&velocity) * dt);

			if (IsMove(nextPosition))
			{
				_float y = GetHeight(nextPosition);
				nextPosition.y = y;

				m_pTransform->SetPosition(nextPosition);
			}
			else
			{
				nextPosition = MakeSlideVector(currPosition, nextPosition);

				if (IsMove(nextPosition))
				{
					_float y = GetHeight(nextPosition);
					nextPosition.y = y;

					m_pTransform->SetPosition(nextPosition);
					m_pTransform->SetPosition(nextPosition);
				}
			}

			m_pTransform->SetForward(targetDir);

			if (m_iCurrPathCursor + 1 < m_CurrPath.cellIndices.size() && m_iCurrCellIndex == m_CurrPath.cellIndices[m_iCurrPathCursor + 1])
				++m_iCurrPathCursor;
		}
	}
}

_float3 NavigationComponent::GetCurrCellNormal()
{
	return m_pNavigationSystem->GetCellNormal(m_iCurrCellIndex);
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