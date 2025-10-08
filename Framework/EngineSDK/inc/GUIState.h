#pragma once

#ifdef USE_IMGUI

NS_BEGIN(Engine)

class Level;
class Layer;
class Object;
class ObjectManager;
class LevelManager;
class CameraManager;

struct GUIState
{
    LevelManager* pLevelManager = nullptr;
    ObjectManager* pObjectManager = nullptr;
    CameraManager* pCameraManager = nullptr;

    Level* pLevel = nullptr;
    Layer* pLayer = nullptr;
    Object* pObject = nullptr;
};

NS_END

#endif