#include "pch.h"
#include "GameManager.h"
#include "EngineCore.h"

IMPLEMENT_SINGLETON(GameManager);

GameManager::GameManager()
{
}

HRESULT GameManager::Initialize()
{
	return S_OK;
}

void GameManager::Update(_float dt)
{
	auto engine = EngineCore::GetInstance();

	switch (m_eCurrBGMState)
	{
	case Client::GameManager::BGM_State::Normal:
	{

	}break;
	case Client::GameManager::BGM_State::FadeToBattle:
	{
		m_fElapsedTime += dt;
		if (m_fElapsedTime >= m_fFadeDuration)
		{
			m_eCurrBGMState = BGM_State::Battle;
			engine->SetChannelVolume(m_iBattleBGMChannel, m_fMasterVolume);
			engine->StopSound(m_iNormalBGMChannel);
			m_iNormalBGMChannel = -1;
			m_fElapsedTime = 0.f;
			return;
		}

		_float t = m_fElapsedTime / m_fFadeDuration;
		t = std::clamp(t, 0.f, 1.f);

		engine->SetChannelVolume(m_iBattleBGMChannel, m_fMasterVolume * t);
		engine->SetChannelVolume(m_iNormalBGMChannel, m_fMasterVolume * (1.f - t));

	}break;
	case Client::GameManager::BGM_State::Battle:
	{

	}break;
	case Client::GameManager::BGM_State::FadeToNormal:
	{
		m_fElapsedTime += dt;
		if (m_fElapsedTime >= m_fFadeDuration)
		{
			m_eCurrBGMState = BGM_State::Normal;
			engine->SetChannelVolume(m_iNormalBGMChannel, m_fMasterVolume);
			engine->StopSound(m_iBattleBGMChannel);
			m_iBattleBGMChannel = -1;
			m_fElapsedTime = 0.f;
			return;
		}

		_float t = m_fElapsedTime / m_fFadeDuration;
		t = std::clamp(t, 0.f, 1.f);
		
		engine->SetChannelVolume(m_iNormalBGMChannel, m_fMasterVolume * t);
		engine->SetChannelVolume(m_iBattleBGMChannel, m_fMasterVolume * (1.f - t));

	}break;
	case Client::GameManager::BGM_State::Count:
		break;
	default:
		break;
	}
}

void GameManager::StartBattle()
{
	if (m_eCurrBGMState == BGM_State::Battle || m_eCurrBGMState == BGM_State::FadeToBattle)
		return;

	auto engine = EngineCore::GetInstance();

	m_iBattleBGMChannel = engine->Play2DSound("BGM_Battle", 0.f);
	
	m_eCurrBGMState = BGM_State::FadeToBattle;
}

void GameManager::StartBossBattle()
{
	if (m_eCurrBGMState == BGM_State::Battle || m_eCurrBGMState == BGM_State::FadeToBattle)
		return;

	auto engine = EngineCore::GetInstance();

	m_fElapsedTime = 0.f;
	m_iBattleBGMChannel = engine->Play2DSound("BGM_Boss", 0.f);
	m_eCurrBGMState = BGM_State::FadeToBattle;
}

void GameManager::EndBattle()
{
	if (m_eCurrBGMState == BGM_State::Normal || m_eCurrBGMState == BGM_State::FadeToNormal)
		return;

	auto engine = EngineCore::GetInstance();

	if (-1 == m_iBattleBGMChannel)
	{
		m_eCurrBGMState = BGM_State::Normal;
		m_iNormalBGMChannel = engine->Play2DSound("BGM_Normal", m_fMasterVolume);
		return;
	}

	m_fElapsedTime = 0.f;
	m_iNormalBGMChannel = engine->Play2DSound("BGM_Normal", 0.f);
	m_eCurrBGMState = BGM_State::FadeToNormal;
}

void GameManager::Free()
{
	__super::Free();
}
