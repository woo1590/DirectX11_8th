#include "pch.h"
#include "Enemy.h"

Enemy::Enemy()
	:ContainerObject()
{
}

Enemy::Enemy(const Enemy& prototype)
	:ContainerObject(prototype)
{
}

void Enemy::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;
}

void Enemy::Free()
{
	__super::Free();
}
