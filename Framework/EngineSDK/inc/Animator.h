#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Bone;
class Animator final:
    public Base
{
private:
    Animator();
    virtual ~Animator() = default;

public:
    static Animator* Create(std::vector<Bone*>& bones);
    HRESULT Initialize(std::vector<Bone*>& bones);

    void Free()override;
    
private:

};

NS_END