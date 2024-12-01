// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyInterface.h"

// Add default functionality here for any IEnemyInterface functions that are not pure virtual.

void IEnemyInterface::GetHurt(int Damage)
{
	Health = Health > Damage ? Health - Damage : 0;
	if (Health == 0)
	{
		Die();
	}
}

void IEnemyInterface::Die()
{
	AActor* Actor = Cast<AActor>(this);
	if (Actor) 
	{
		Actor->Destroy();
	}
}
