// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyHealthComponent.h"

// Sets default values for this component's properties
UEnemyHealthComponent::UEnemyHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemyHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnemyHealthComponent::GetHurt(int Damage)
{
	Health = Health > Damage ? Health - Damage : 0;
	if (Health == 0)
	{
		Die();
	}
}

void UEnemyHealthComponent::Die()
{
	GetOwner()->Destroy();
}

