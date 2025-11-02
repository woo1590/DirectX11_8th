#include "pch.h"
#include "SkyBox.h"
#include "EngineCore.h"
#include "VIBufferCube.h"
#include "Material.h"

SkyBox::SkyBox()
    :Object()
{
}

SkyBox::SkyBox(const SkyBox& prototype)
    :Object(prototype)
{
}

SkyBox* SkyBox::Create()
{
    SkyBox* Instance = new SkyBox();

    if (FAILED(Instance->Initialize_Prototype()))
        Safe_Release(Instance);

    return Instance;
}

HRESULT SkyBox::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    m_strInstanceTag = "Skybox";
    m_eRenderGroup = RenderGroup::Priority;

    return S_OK;
}

HRESULT SkyBox::Initialize(InitDESC* arg)
{
    if (FAILED(__super::Initialize(arg)))
        return E_FAIL;

    auto engine = EngineCore::GetInstance();

    m_pVIBuffer = engine->GetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Cube");
    m_pMaterial = engine->GetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_Skybox");

    m_pVIBuffer->AddRef();
    m_pMaterial->AddRef();

    return S_OK;
}

void SkyBox::PriorityUpdate(_float dt)
{
    __super::PriorityUpdate(dt);
}

void SkyBox::Update(_float dt)
{
    __super::Update(dt);

}

void SkyBox::LateUpdate(_float dt)
{
    __super::LateUpdate(dt);

    CAMERA_CONTEXT camContext = EngineCore::GetInstance()->GetCameraContext();
    m_pTransform->SetPosition(camContext.camPosition);
}

HRESULT SkyBox::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
    RenderProxy proxy{};
    proxy.buffer = m_pVIBuffer;
    proxy.material = m_pMaterial;
    proxy.worldMatrix = m_pTransform->GetWorldMatrix();

    proxies[ENUM_CLASS(m_eRenderGroup)].push_back(proxy);

    return S_OK;
}

Object* SkyBox::Clone(InitDESC* arg)
{
    SkyBox* Instance = new SkyBox(*this);

    if (FAILED(Instance->Initialize(arg)))
        Safe_Release(Instance);

    return Instance;
}

void SkyBox::Free()
{
    __super::Free();

    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pMaterial);
}
