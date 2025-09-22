#pragma once
#include "Base.h"

NS_BEGIN(Importer)

class FBXBone;
class FBXSkeleton :
    public Base
{
private:
    FBXSkeleton();
    virtual ~FBXSkeleton() = default;

public:
    static FBXSkeleton * Create(const aiScene* pScene);
    HRESULT Initialize(const aiScene* pScene);
    void UpdateCombiendTransformation();

    void Export(std::ofstream& out);

    _int GetBoneIndexByName(const _string& name);
    _matrix GetCombinedTransformMatrix(_uint index);
    std::vector<FBXBone*>& GetBones() { return m_Bones; }

    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector();
#endif

private:
    HRESULT ProcessNode(aiNode* childNode, _int parentIndex);

    _int m_iNumBones{};
    std::vector<FBXBone*> m_Bones;

};

NS_END