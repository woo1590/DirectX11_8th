#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class Object;
class ColliderComponent;
NS_END

NS_BEGIN(MapEditor)

enum class PickType { Guizmo, Model, Chunk, Nav, Spawner, Count };

typedef struct tagPickResult
{
	bool isHit = false;
	float distance = FLT_MAX;
	PickType type{};

	/*Model Pick*/
	class Object* object = nullptr;
	XMFLOAT3 worldHitPosition{};

	/*Chunk Pick*/
	unsigned int indexX{};
	unsigned int indexZ{};
	XMFLOAT3 chunkPosition{};
	bool isUsed = false;

	/*NavCell Pick*/
	_int navCellIndex = -1;
	_int navCellPointIndex = -1;

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
	void RegisterComponent(ColliderComponent* component);
	void UnRegisterComponent(PickableComponent* component);
	void UnRegisterComponent(ColliderComponent* component);
	PICK_RESULT GetLastPickResult()const { return m_LastPickResult; }
    void Free()override;

private:
	void RayCast(RAY ray);

	std::list<PickableComponent*> m_Pickables;
	std::list<ColliderComponent*> m_Colliders;
	PICK_RESULT m_LastPickResult{};
};

NS_END