#include "pch.h"
#include "EffectEditorLevel.h"
#include "EngineCore.h"
#include "VIBufferCube.h"
#include "VIBufferQuad.h"
#include "VIBuffer_Point.h"
#include "Socket.h"
#include "Editor_EffectContainer.h"
#include "Editor_EffectNode.h"
#include "EffectEditorCamera.h"
#include "EffectEditorGrid.h"
#include "EffectEditorLight.h"

#include "Editor_SpriteEffect.h"

EffectEditorLevel::EffectEditorLevel()
	:Level()
{
}

EffectEditorLevel* EffectEditorLevel::Create()
{
	EffectEditorLevel* Instance = new EffectEditorLevel();

	if (FAILED(Instance->Intialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT EffectEditorLevel::Intialize()
{
    Initialize_Resource();
    Initialize_Object();

	return S_OK;
}

void EffectEditorLevel::Update(_float dt)
{
}

HRESULT EffectEditorLevel::Render()
{
	return S_OK;
}

HRESULT EffectEditorLevel::Initialize_Resource()
{
    auto engine = EngineCore::GetInstance();

    /*Load Shader*/
    {
        if (FAILED(engine->LoadShaderFromFile("../bin/shaderfiles/Shader_VtxTex.hlsl", "Shader_VtxTex",
            VTXTEX::elements, VTXTEX::numElement)))
            return E_FAIL;

        if (FAILED(engine->LoadShaderFromFile("../bin/shaderfiles/Shader_Grid.hlsl", "Shader_Grid",
            VTXTEX::elements, VTXTEX::numElement)))
            return E_FAIL;

        if (FAILED(engine->LoadShaderFromFile("../bin/shaderfiles/Shader_VtxCube.hlsl", "Shader_VtxCube",
            VTXCUBE::elements, VTXCUBE::numElement)))
            return E_FAIL;

        if (FAILED(engine->LoadShaderFromFile("../bin/shaderfiles/Shader_VtxNorTex.hlsl", "Shader_VtxNorTex",
            VTXNORTEX::elements, VTXNORTEX::numElement)))
            return E_FAIL;

        if (FAILED(engine->LoadShaderFromFile("../bin/shaderfiles/Shader_VtxMesh.hlsl", "Shader_VtxMesh",
            VTXMESH::elements, VTXMESH::numElement)))
            return E_FAIL;

        if (FAILED(engine->LoadShaderFromFile("../bin/shaderfiles/Shader_VtxSkinnedMesh.hlsl", "Shader_VtxSkinnedMesh",
            VTXSKINNEDMESH::elements, VTXSKINNEDMESH::numElement)))
            return E_FAIL;

        if (FAILED(engine->LoadShaderFromFile("../bin/shaderfiles/Shader_VtxPoint.hlsl", "Shader_VtxPoint",
            VTXPOS::elements, VTXPOS::numElement)))
            return E_FAIL;
    }
    /*Load Buffer*/
    {
        if (FAILED(engine->LoadBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad", VIBufferQuad::Create())))
            return E_FAIL;

        if (FAILED(engine->LoadBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Cube", VIBufferCube::Create())))
            return E_FAIL;

        if (FAILED(engine->LoadBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Point", VIBuffer_Point::Create())))
            return E_FAIL;
    }
    /*Load Material*/
    {
        if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/grid.json",
            "Mtrl_Grid")))
            return E_FAIL;
    }
    /*Load Prototype Object*/
    {
        if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Camera", EffectEditorCamera::Create())))
            return E_FAIL;
        if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Grid", EffectEditorGrid::Create())))
            return E_FAIL;
        if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Light", EffectEditorLight::Create())))
            return E_FAIL;
        if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_EffectContainer", Editor_EffectContainer::Create())))
            return E_FAIL;
        if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_SpriteEffect", Editor_SpriteEffect::Create())))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT EffectEditorLevel::Initialize_Object()
{
	auto engine = EngineCore::GetInstance();

    if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Camera", ENUM_CLASS(LevelID::EffectEditor), "Layer_Camera")))
        return E_FAIL;
    if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Grid", ENUM_CLASS(LevelID::EffectEditor), "Layer_Grid")))
        return E_FAIL;
    if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Light", ENUM_CLASS(LevelID::EffectEditor), "Layer_Light")))
        return E_FAIL;
    if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_EffectContainer", ENUM_CLASS(LevelID::EffectEditor), "Layer_Effect")))
        return E_FAIL;

	return S_OK;
}
