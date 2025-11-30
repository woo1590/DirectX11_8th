#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class WinMode { Full, Win, Count };
	enum class Prototype { Object, Component, Count };
	enum class RenderGroup { Priority, Shadow, NonBlend, NonLight, Blend, NavMeshDebug, ColliderDebug, UI, CustomPass, None, Count };
	enum class MouseButton{LButton, RButton, Count};
	enum class LightType{Directional, Point, Spot, Count};
	enum class ModelType{Static, Skinned, Count};
	enum class PickingType { Triangle, BoundingBox, Count };
	enum class NavCellPoint { A, B, C, Count };
	enum class NavCellLine { AB, BC, CA, Count };
	enum class ColliderType { AABB, OBB, Sphere, Count };

	enum class ParticleSpace { Local, World, Count };
	enum class ParticleDirMode { None, SurfaceDir, SwayUp, Count };

	enum class TrailMode { Default, Ribbon, Count };
}
#endif // Engine_Enum_h__
