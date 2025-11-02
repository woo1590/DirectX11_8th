#pragma once

#ifdef USE_IMGUI

NS_BEGIN(Engine)

class Level;
class Layer;
class Object;
class ObjectManager;
class LevelManager;
class CameraManager;
class RenderTargetManager;

struct GUIState
{
    LevelManager* pLevelManager = nullptr;
    ObjectManager* pObjectManager = nullptr;
    CameraManager* pCameraManager = nullptr;
    RenderTargetManager* pRenderTargetManager = nullptr;

    Level* pLevel = nullptr;
    Layer* pLayer = nullptr;
    Object* pObject = nullptr;
};

NS_END

#endif