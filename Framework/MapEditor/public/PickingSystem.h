#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class Object;
NS_END

NS_BEGIN(MapEditor)

enum class PickType { Model, Chunk, Count };

typedef struct tagRay
{
	_float3 position{};
	_float3 direction{};
}RAY;

typedef struct tagPickResult
{
	_bool isHit = false;
	_float distance = FLT_MAX;
	PickType type{};

	/*Model Pick*/
	Object* object = nullptr;

	/*Chunk Pick*/
	_uint indexX{};
	_uint indexZ{};

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
	PICK_RESULT GetLastPickResult()const { return m_LastPickResult; }
    void Free()override;

private:
	void RayCast(RAY ray);

	std::vector<PickableComponent*> m_Pickables;
	PICK_RESULT m_LastPickResult{};
};

NS_END