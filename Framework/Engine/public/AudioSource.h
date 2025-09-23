#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL AudioSource :
    public Component
{
private:
    AudioSource(Object* owner);
    AudioSource(const AudioSource& prototype);
    virtual ~AudioSource() = default;

public:
    static AudioSource* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void Update(_float dt)override;
    
    Component* Clone()override { return new AudioSource(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    
};

NS_END