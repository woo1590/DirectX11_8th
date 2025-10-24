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

void Enemy::Free()
{
	__super::Free();
}
