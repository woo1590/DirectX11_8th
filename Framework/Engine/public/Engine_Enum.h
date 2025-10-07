#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class WinMode { Full, Win, Count };
	enum class Prototype { Object, Component, Count };
	enum class RenderGroup { Priority, Shadow, NonBlend, Blend, NavMeshDebug, ColliderDebug, UI, Count };
	enum class MouseButton{LButton, RButton, Count};
	enum class LightType{Directional, Point, Spot, Count};
	enum class ModelType{Static, Skinned, Count};
	enum class PickingType { Triangle, BoundingBox, Count };
	enum class CellPoint { A, B, C, Count };
}
#endif // Engine_Enum_h__
