// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Attacks/Attack.h"
#include "Character/Attacks/WilloWisp.h"
#include "TimerManager.h"
#include "WilloWispSpawning.generated.h"

class UPlayerAttackManger;

/**
 * 
 */
UCLASS()
class ASHORTSTAYINLIBRARY_API UWilloWispSpawning : public UAttack
{
	GENERATED_BODY()

public: 
	enum class WilloWispStatus
	{
		NotSpawned, Spawning, Spawned
	};

	UWilloWispSpawning();
	UWilloWispSpawning(FString name, int Damage, float CoolDownInSeconds, int EnergyNeeded);

	void SetUp(FString WilloWispName, float WilloWispEnergyNeeded, float WilloWispCoolDownInSeconds, int Number, TArray<FVector>& Offsets, TArray<AWilloWisp*>* WilloWisps, FTimerManager* Manager, TSubclassOf<AActor>* WilloWispBP, float WilloWispEnergy);
	void StartAttack(FAttackParams AttackParams) override;

protected:
	void SpawnWilloWisps(FAttackParams AttackParams);
	void OnSpawnTimerElapsed();

	int NumberOfWilloWisps;
	TArray<FVector> WilloWispsOffSets;
	TArray<AWilloWisp*>* SpawnedWilloWisps;
	bool HasSpawned = false;
	bool IsSpawning = false;
	WilloWispStatus WilloWispState = WilloWispStatus::NotSpawned;
	bool IsInCoolDown = false;
	float WilloWispEnergyIndividual;

	FTimerManager* TimerManager;
	FTimerHandle TimerHandle;
	FAttackParams AttackParams;
	UPlayerAttackManger* AttackManager;
	TSubclassOf<AActor>* WilloWispBluePrint;
};
