#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL AudioListener :
    public Component
{
private:
    AudioListener(Object* owner);
    AudioListener(const AudioListener& prototype);
    virtual ~AudioListener() = default;

public:
    static AudioListener* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    Component* Clone() { return new AudioListener(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:

};

NS_END