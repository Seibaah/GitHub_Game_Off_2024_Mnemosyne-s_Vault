// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Attacks/Attack.h"
#include "Character/Attacks/WilloWisp.h"
#include "WilloWispSending.generated.h"

/**
 * 
 */
UCLASS()
class ASHORTSTAYINLIBRARY_API UWilloWispSending : public UAttack
{
	GENERATED_BODY()

	friend class AMainChracter; 

public: 
	UWilloWispSending();
	UWilloWispSending(FString Name, int Damage, float CoolDownInSeconds, int EnergyNeeded);

	void PostInitProperties() override;
	void SetUp(FTimerHandle* SpawningTimerHandle, FTimerManager* TimerManager, TArray<AWilloWisp*>* WilloWisps, float CoolDown);

protected:
	void StartAttack(FAttackParams AttackParams) override;
	FTimerHandle* SpawningTimerHandle;
	FTimerManager* TimerManager; 
	TArray<AWilloWisp*>* SpawnedWilloWisps;
	float WilloWispSpawningCoolDown = 1.f; 
};
