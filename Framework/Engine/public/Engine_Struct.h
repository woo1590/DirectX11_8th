#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"
#include "Struct_ConstantBuffer.h"
#include "Struct_Vertex.h"

namespace Engine
{
	struct EngineDESC
	{
		HWND hWnd;
		WinMode winMode;
		_uint winSizeX, winSizeY;
		_uint levelCnt;
	};

	struct InitDESC {};

	typedef struct tagKeyFrame
	{
		_float3 scale{};
		_float4 rotation{};
		_float3 position{};
		_float trackPosition{};

	}KEYFRAME;

	typedef struct tagAnimationSet
	{
		_uint numAnimations;
		std::vector<class AnimationClip*> aniamtionClips;

	}ANIMATION_SET;

	typedef struct tagAnimationClipContext
	{
		_float trackPosition{};
		std::vector<_uint> keyFrameIndices;
		_bool isLoop = false;
		_bool isFinished = false;

	}ANIMATIONCLIP_CONTEXT;

	typedef struct tagBonePalette
	{
		std::vector<_float4x4> boneMatrices;
	}BONE_PALETTE;

	typedef struct tagPrefab
	{
		_string prototypeTag{};
		_string modelTag{};
		_string layerTag{};

		_float3 position{};
		_float3 scale{ 1.f,1.f,1.f };
		_float3 rotation{};
	}PREFAB;
}
#endif // Engine_Struct_h__
