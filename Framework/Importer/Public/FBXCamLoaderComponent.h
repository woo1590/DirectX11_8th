#pragma once
#include "Component.h"

NS_BEGIN(Importer)

class FBXCamLoaderComponent :
    public Component
{
private:
    FBXCamLoaderComponent(Object* owner);
    FBXCamLoaderComponent(const FBXCamLoaderComponent& prototype);
    virtual ~FBXCamLoaderComponent() = default;

public:
    static FBXCamLoaderComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* desc)override;

    HRESULT ImportCamera(const _string& filePath);
    HRESULT ExportCamera(const _string& outFilePath);

    Component* Clone()override { return new FBXCamLoaderComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector() override;
    void ImportInspector(_string opendFile);
    void ExportInspector(_string savedFileName);
#endif

private:
    _bool m_isLoaded = false;
};

NS_END