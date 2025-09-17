#include "EnginePCH.h"
#include "Utility_Function.h"

HRESULT ENGINE_DLL DXWrap::EngineCreateWICTextureFromFile(ID3D11Device* device, _wstring fileName, ID3D11Resource** ppResource, ID3D11ShaderResourceView** ppSRV)
{
	return CreateWICTextureFromFile(device, fileName.c_str(), ppResource, ppSRV);
}

HRESULT ENGINE_DLL DXWrap::EngineSaveDDSTextureToFile(ID3D11DeviceContext* context, ID3D11Resource* pResource, _wstring outFilePath)
{
	return SaveDDSTextureToFile(context, pResource, outFilePath.c_str());
}
