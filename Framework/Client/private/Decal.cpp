#include "pch.h"
#include "Decal.h"
#include "MaterialInstance.h"
#include "SpriteComponent.h"

Decal::Decal()
    :Object()
{
}

Decal::Decal(const Decal& prototype)
    :Object(prototype)
{
}

Decal* Decal::Create()
{
    Decal* Instance = new Decal();

    if (FAILED(Instance->Initialize_Prototype()))
        Safe_Release(Instance);

    return Instance;
}

HRESULT Decal::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    AddComponent<SpriteComponent>();

    m_eRenderGroup = RenderGroup::Blend;

    return S_OK;
}

HRESULT Decal::Initialize(InitDESC* arg)
{
    if (FAILED(__super::Initialize(arg)))
        return E_FAIL;
    
    auto engine = EngineCore::GetInstance();

    /*sprite*/  
    SpriteComponent::SPRITE_DESC spriteDesc{};
    spriteDesc.isAnimated = false;
    spriteDesc.iMaxFrameIndex = 1;

    auto sprite = GetComponent<SpriteComponent>();
    sprite->Initialize(&spriteDesc);

    sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Point");
    sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_Decal");

    auto mtrlInstance = sprite->GetMaterialInstance();
    mtrlInstance->SetPass("Decal_Pass");

    DECAL_DESC* desc = static_cast<DECAL_DESC*>(arg);

    _float3 surface{};
    XMStoreFloat3(&surface, XMLoadFloat3(&desc->surfaceDir) * -1.f);

    m_pTransform->SetForward(surface);
    m_pTransform->SetScale(_float3{ 2.f,2.f,2.f });

    return S_OK;
}

void Decal::PriorityUpdate(_float dt)
{
    __super::PriorityUpdate(dt);
}

void Decal::Update(_float dt)
{
    __super::Update(dt);

    m_fElapsedTime += dt;
    if (m_fElapsedTime >= m_fDuration)
        SetDead();

    //GetComponent<SpriteComponent>()->GetMaterialInstance()->SetFloat("g_DecalProgress", m_fElapsedTime / m_fDuration);
}

void Decal::LateUpdate(_float dt)
{
    __super::LateUpdate(dt);
}

HRESULT Decal::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
    __super::ExtractRenderProxies(proxies);

    return S_OK;
}

Object* Decal::Clone(InitDESC* arg)
{
    Decal* Instance = new Decal(*this);

    if (FAILED(Instance->Initialize(arg)))
        Safe_Release(Instance);

    return Instance;
}

void Decal::Free()
{
    __super::Free();
}
