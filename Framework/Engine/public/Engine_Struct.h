#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"
#include "Struct_ConstantBuffer.h"
#include "Struct_Vertex.h"
#include "Struct_ShaderMetaData.h"
#include "Struct_BVHNode.h"
#include "Struct_NavMesh.h"

namespace Engine
{
	struct EngineDESC
	{
		HWND hWnd;
		WinMode winMode;
		_uint winSizeX, winSizeY;
		_uint levelCnt;
		_uint numCollisionFilter;
		_uint numEvent;
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
		_float playSpeedScale = 1.f;

	}ANIMATIONCLIP_CONTEXT;

	typedef struct tagBonePalette
	{
		std::vector<_float4x4> boneMatrices;
	}BONE_PALETTE;

	typedef struct tagBoneMap
	{
		std::vector<_int> mappedBoneIndex;
		std::unordered_map<_string, _int> nameToIndex;
	}BONE_MAP;

	typedef struct tagBone
	{
		_string m_strBoneTag{};
		_int m_iParentIndex{};
		_float4x4 m_TransformationMatrix{};

	}Bone;

	typedef struct tagPrefab
	{
		_string prototypeTag{};
		_string modelTag{};
		_string layerTag{};

		_float3 position{};
		_float3 scale{ 1.f,1.f,1.f };
		_float4 quaternion{};	//quaternion
	}PREFAB;

	typedef struct tagBoundingBoxData
	{
		_float3 AABBMin{};
		_float3 AABBMax{};
	}BOUNDING_BOX_DATA;

	typedef struct tagRay
	{
		_float3 origin{};
		_float3 direction{};
	}RAY;

	typedef struct tagRayCastData
	{
		_bool isHit = false;
		_float worldDistance = FLT_MAX;
		_float3 hitNormal{};
		class Object* hitObject = nullptr;
	}RAYCAST_DATA;

	typedef struct tagRayHitData
	{
		_bool isHit = false;
		_float localDistance = FLT_MAX;
		_float3 normal{};
	}RAY_HIT_DATA;

	typedef struct tagCameraContext 
	{
		_float4x4 viewMatrix{};
		_float4x4 viewMatrixInverse{};

		_float4x4 projMatrix{};
		_float4x4 projMatrixInverse{};

		_float3 camPosition{};

		_float farZ{};
		_float nearZ{};
	}CAMERA_CONTEXT;

	typedef struct tagEvent
	{
		_uint eventID{};
		std::any param{};

		tagEvent(_uint id, std::any param)
			:eventID(id), param(param) {
		}

	}EVENT;

	typedef struct tagListener
	{
		const class EventHandler* handler = nullptr;
		std::function<void(std::any)> eventFunction;

		tagListener(const EventHandler* handler,const std::function<void(std::any)>& function)
			:handler(handler), eventFunction(function) {
		}

	}LISTENER;
	
}
#endif // Engine_Struct_h__
