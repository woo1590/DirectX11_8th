#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class Object;
NS_END

NS_BEGIN(MapEditor)

enum class PickType { Guizmo, Model, Chunk, Count };

typedef struct tagPickResult
{
	_bool isHit = false;
	_float distance = FLT_MAX;
	PickType type{};

	/*Model Pick*/
	Object* object = nullptr;
	_float3 worldHitPosition{};

	/*Chunk Pick*/
	_uint indexX{};
	_uint indexZ{};
	_float3 chunkPosition{};
	_bool isUsed = false;

}PICK_RESULT;

class PickableComponent;
class PickingSystem :
    public Base
{
private:
    PickingSystem();
    virtual ~PickingSystem() = default;

public:
    static PickingSystem* Create();
    HRESULT Initialize();
	void Update();
    void RegisterComponent(PickableComponent* component);
	void UnRegisterComponent(PickableComponent* component);
	PICK_RESULT GetLastPickResult()const { return m_LastPickResult; }
    void Free()override;

private:
	void RayCast(RAY ray);

	std::list<PickableComponent*> m_Pickables;
	PICK_RESULT m_LastPickResult{};
};

NS_END