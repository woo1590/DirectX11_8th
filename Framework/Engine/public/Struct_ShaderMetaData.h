#pragma once

namespace Engine
{
	typedef struct tagVariableMeta
	{
		_string name{};
		_string varType{};

		_uint offset{};
		_uint size{};

		/*for float4x4*/
		_uint rows{};
		_uint cols{};

		/*for array*/
		_uint elements;

	}VARIABLE_META;

	typedef struct tagGlobalVariableMeta
	{
		_string name{};
		_string varType{};

		_uint offset{};
		_uint size{};
		_uint slot{};

		/*for float4x4*/
		_uint rows{};
		_uint cols{};

		/*for array*/
		_uint elements;

		/*cached handle*/
		ID3DX11EffectVariable* variableHandle = nullptr;

	}GLOBAL_VARIABLE_META;

	typedef struct tagResourceMeta
	{
		_string name{};
		_string typeName{};
		_uint slot{};

		/*cached handle*/
		ID3DX11EffectShaderResourceVariable* srvHandle = nullptr;

	}RESOURCE_META;

	typedef struct tagCBufferMeta
	{
		_string name{};
		_uint size;
		_uint slot;
		std::vector<VARIABLE_META> variables;

		/*cached handle*/
		ID3DX11EffectConstantBuffer* bufferHandle = nullptr;

	}CBUFFER_META;
}