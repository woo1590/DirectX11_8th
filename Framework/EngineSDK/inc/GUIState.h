#pragma once
#include "EnginePCH.h"

#ifdef USE_IMGUI

NS_BEGIN(Engine)

class Level;
class Layer;
class Object;
class ObjectManager;
class PipeLine;

struct GUIState
{
    ObjectManager* pObjectManager = nullptr;
    PipeLine* pPipeLine = nullptr;

    Level* pLevel = nullptr;
    Layer* pLayer = nullptr;
    Object* pObject = nullptr;

};

NS_END

#endif