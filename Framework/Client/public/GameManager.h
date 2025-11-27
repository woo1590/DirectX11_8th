#pragma once
#include "Base.h"

NS_BEGIN(Client)

class GameManager :
    public Base
{
    DECLARE_SINGLETON(GameManager);
private:
    enum class BGM_State { Normal, FadeToBattle, Battle, FadeToNormal, Count };

    GameManager();
    virtual ~GameManager() = default;

public:
    HRESULT Initialize();
    void Update(_float dt);

    void StartBattle();
    void StartBossBattle();
    void EndBattle();

    void Free()override;
private:
    _int m_iNormalBGMChannel = -1;
    _int m_iBattleBGMChannel = -1;
    _int m_iBossBGMChannel = -1;
    _float m_fElapsedTime{};
    _float m_fFadeDuration = 1.f;

    BGM_State m_eCurrBGMState = BGM_State::Count;
    _float m_fMasterVolume = 0.2f;
};

NS_END