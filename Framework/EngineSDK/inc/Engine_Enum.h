#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class WinMode { Full, Win, Count };
	enum class Prototype { Object, Component, Count };
	enum class RenderGroup { Priority, Shadow, NonBlend, Blend, UI, Count };
	enum class MouseButton{LButton, RButton, Count};
	enum class LightType{Directional, Point, Spot, Count};
}
#endif // Engine_Enum_h__
