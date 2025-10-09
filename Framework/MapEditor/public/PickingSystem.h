#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class Object;
NS_END

NS_BEGIN(MapEditor)

enum class PickType { Guizmo, Model, Chunk, Count };

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
	std::pair<_uint, _uint> edge;

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