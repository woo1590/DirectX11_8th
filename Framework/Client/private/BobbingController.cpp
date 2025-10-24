#include "pch.h"
#include "BobbingController.h"
#include "EngineCore.h"
#include "Random.h"

BobbingController::BobbingController()
{
}

BobbingController* BobbingController::Create()
{
	BobbingController* Instance = new BobbingController();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT BobbingController::Initialize()
{
	OnIdle();

	return S_OK;
}

void BobbingController::Update(_float dt, _float3& posAdd, _float3& rotAdd)
{
	UpdateGround(dt, posAdd, rotAdd);
	UpdateAir(dt, posAdd, rotAdd);
	UpdateRecoil(dt, posAdd, rotAdd);
}

void BobbingController::OnIdle()
{
	m_fTargetWeight = 0.5f;
	m_fTargetFreq = m_fIdleFreq;
	m_TargetOffset = m_IdleOffset;
	m_TargetRot = m_IdleRot;
}

void BobbingController::OnWalk()
{
	m_fTargetWeight = 0.8f;
	m_fTargetFreq = m_fWalkFreq;
	m_TargetOffset = m_WalkOffset;
	m_TargetRot = m_WalkRot;
}

void BobbingController::StartJump()
{
	m_JumpState.isJump = true;
	m_JumpState.onAir = true;
	m_JumpState.elapsedTime = 0.f;
	m_JumpState.weight = 0.f;
}

void BobbingController::EndJump()
{
	m_JumpState.isJump = false;
	m_JumpState.onAir = false;
	m_JumpState.isLand = true;
	m_JumpState.elapsedTime = 0.f;
	m_JumpState.weight = 0.f;
}

void BobbingController::AddRecoil(_float power)
{
	m_RecoilState.isShot = true;
	m_RecoilState.velocity = power;
}

void BobbingController::Free()
{
	__super::Free();
}

void BobbingController::UpdateGround(_float dt, _float3& posAdd, _float3& rotAdd)
{
	m_fCurrWeight = FollowExp(m_fCurrWeight, m_fTargetWeight, dt);
	m_fCurrFreq = FollowExp(m_fCurrFreq, m_fTargetFreq, dt);

	_float t = (1.f - std::exp(-m_fFollow * dt));

	m_CurrOffset.x = math::Lerp(m_CurrOffset.x, m_TargetOffset.x, t);
	m_CurrOffset.y = math::Lerp(m_CurrOffset.y, m_TargetOffset.y, t);
	m_CurrOffset.z = math::Lerp(m_CurrOffset.z, m_TargetOffset.z, t);

	m_CurrRot.x = math::Lerp(m_CurrRot.x, m_TargetRot.x, t);
	m_CurrRot.y = math::Lerp(m_CurrRot.y, m_TargetRot.y, t);

	m_fPhase += math::TWO_PI * m_fCurrFreq * dt;
	if (m_fPhase >= math::TWO_PI)
		m_fPhase = std::fmodf(m_fPhase, math::TWO_PI);

	_float s = std::sinf(m_fPhase);
	_float c = std::cosf(m_fPhase);

	_float x = m_CurrOffset.x * s;
	_float y = m_CurrOffset.y * s;
	_float z = m_CurrOffset.z * (1.f - c);

	_float pitch = m_CurrRot.x * s;
	_float yaw = m_CurrRot.y * c;

	posAdd.x = m_fCurrWeight * x;
	posAdd.y = m_fCurrWeight * y;
	posAdd.z = m_fCurrWeight * z;

	rotAdd.x = m_fCurrWeight * pitch;
	rotAdd.y = m_fCurrWeight * yaw;
}

void BobbingController::UpdateAir(_float dt, _float3& posAdd, _float3& rotAdd)
{
	if (m_JumpState.onAir)
	{
		if (m_JumpState.isJump)
		{
			m_JumpState.elapsedTime += dt;

			_float t0 = m_JumpState.elapsedTime / m_JumpParams.airDuration;
			_float t1 = m_JumpState.elapsedTime / m_JumpParams.upDuration;

			/*start jump event*/
			if (t1 >= 1.f)
				m_JumpState.isJump = false;
			else
			{
				t1 = math::EaseOutSline(t1);
				rotAdd.x += math::PalabolaCurve(t1, m_JumpParams.upPitch);
			}

			/*on air evnet*/
			_float airTarget = math::SmoothStep(t0);
			m_JumpState.weight = FollowExp(m_JumpState.weight, airTarget, dt);

			rotAdd.x += m_JumpParams.airPitchMax * m_JumpState.weight;
			posAdd.z += m_JumpParams.airZMax * m_JumpState.weight;

		}
		else
		{
			m_JumpState.elapsedTime += dt;

			_float t = m_JumpState.elapsedTime / m_JumpParams.airDuration;
			t = math::SmoothStep(t);

			m_JumpState.weight = FollowExp(m_JumpState.weight, t, dt);

			posAdd.z += m_JumpParams.airZMax * m_JumpState.weight;
			rotAdd.x += m_JumpParams.airPitchMax * m_JumpState.weight;
		}
	}

	if (m_JumpState.isLand)
	{
		m_JumpState.elapsedTime += dt;

		_float t = m_JumpState.elapsedTime / m_JumpParams.landDuration;
		if (t >= 1.f)
			m_JumpState.isLand = false;
		else
		{
			t = math::EaseOutSline(t);

			rotAdd.x += math::PalabolaCurve(t, m_JumpParams.landPitch);
			posAdd.z += m_JumpParams.landZ * (-4.f * 1.f * m_JumpState.weight * (1.f - m_JumpState.weight));
		}
	}
}

void BobbingController::UpdateRecoil(_float dt, _float3& posAdd, _float3& rotAdd)
{
	_float t = 20.f * 20.f;
	_float c = 0.5f * 20.f * 2.f;

	m_RecoilState.velocity += (-t * m_RecoilState.positionZ - c * m_RecoilState.velocity) * dt;
	m_RecoilState.positionZ = m_RecoilState.velocity * dt;	

	posAdd.z += m_RecoilState.positionZ;
	posAdd.y -= m_RecoilState.positionZ * 0.5f;

}

_float BobbingController::FollowExp(_float t, _float target, _float dt)
{
	return t + (1.f - std::exp(-m_fFollow * dt)) * (target - t);
}
