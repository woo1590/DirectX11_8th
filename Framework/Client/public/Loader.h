#pragma once
#include "Base.h"

NS_BEGIN(Client)

class Loader final:
    public Base
{
private:
    Loader();
    virtual ~Loader() = default;

public:
    static Loader* Create(LevelID levelID);
    HRESULT Initialize(LevelID levelID);
    void Free()override;

public:
    void DebugPrint();
    _bool IsFinish()const { return m_isFinished.load(); }

private:
    HRESULT Loading();

    HRESULT LoadingForLogo();
    HRESULT LoadingForGamePlay();
    HRESULT LoadingForTest();
    HRESULT LoadingForStage1();
    HRESULT LoadingForBossStage();

    LevelID m_eNextLevelID{};

    std::thread m_Worker{};
    std::mutex m_Mutex{};                       //임계영역 생성
    std::atomic<_bool> m_isFinished = false;  //로딩 스레드에서 true로 변환해도 메인스레드에서 true로 읽어들인다는 보장이 없음 - 메모리 가시성
                                            /*원자성 보장 - 연산의 중간 과정이 보이지 않음
                                              Race Condition 방지 - 동기화된 접근이 보장됨
                                              메모리 가시성 - 어떤 스레드에서 읽어도 동일한 값이 읽힘*/

    _wstring m_strDebugText{};

};

NS_END