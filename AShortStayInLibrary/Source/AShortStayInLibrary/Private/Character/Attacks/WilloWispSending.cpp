// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attacks/WilloWispSending.h"

UWilloWispSending::UWilloWispSending()
{
}

UWilloWispSending::UWilloWispSending(FString Name, int Damage, float CoolDownInSeconds, int EnergyNeeded) : UAttack(Name, Damage, CoolDownInSeconds, EnergyNeeded)
{
}

void UWilloWispSending::PostInitProperties()
{
	Super::PostInitProperties();

	Name = "WilloWispSending";
	Damage = 5.f;
	CoolDownInSeconds = 0.3f; // Cool down for spawning after all willowisps are sent out
	EnergyNeeded = 0;
}

void UWilloWispSending::SetUp(FTimerHandle* Handle, FTimerManager* Manager, TArray<AWilloWisp*>* WilloWisps, float CoolDown)
{
	SpawningTimerHandle = Handle;
	ensure(Manager);
	TimerManager = Manager;
	SpawnedWilloWisps = WilloWisps;
	WilloWispSpawningCoolDown = CoolDown;
}

void UWilloWispSending::StartAttack(FAttackParams AttackParams)
{
	ensure(!SpawnedWilloWisps->IsEmpty());
	AWilloWisp* WilloWisp = (*SpawnedWilloWisps)[0];
	WilloWisp->Fire(AttackParams.ShootDirection, AttackParams.Lantern->GetFireStartPos());
	SpawnedWilloWisps->Remove(WilloWisp);
	if (SpawnedWilloWisps->IsEmpty()) 
	{
		TimerManager->SetTimer(*SpawningTimerHandle, 1, false);
	}
}
