// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attacks/WilloWispSpawning.h"

UWilloWispSpawning::UWilloWispSpawning()
{
}

UWilloWispSpawning::UWilloWispSpawning(FString Name, int Damage, float CoolDownInSeconds, int EnergyNeeded) : UAttack(Name, Damage, CoolDownInSeconds, EnergyNeeded)
{
}

void UWilloWispSpawning::SetUp(FString WilloWispName, float WilloWispEnergyNeeded, float WilloWispCoolDownInSeconds, int Number, TArray<FVector>& Offsets, TArray<AWilloWisp*>* WilloWisps, FTimerManager* Manager, TSubclassOf<AActor>* WilloWispBP, float WilloWispEnergy)
{
	Name = WilloWispName;
	CoolDownInSeconds = WilloWispCoolDownInSeconds;
	EnergyNeeded = WilloWispEnergyNeeded;
	NumberOfWilloWisps = Number;
	WilloWispsOffSets = Offsets;
	SpawnedWilloWisps = WilloWisps;
	TimerManager = Manager;
	TimerHandle = FTimerHandle();
	WilloWispBluePrint = WilloWispBP;
	WilloWispEnergyIndividual = WilloWispEnergy;
}

void UWilloWispSpawning::StartAttack(FAttackParams CurrentAttackParams)
{
	AttackParams = CurrentAttackParams;
	if (SpawnedWilloWisps->IsEmpty() && !TimerManager->IsTimerActive(TimerHandle) && !TimerManager->IsTimerPaused(TimerHandle))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("WilloWisps StartAttack Called"));
		IsSpawning = true;
		TimerManager->SetTimer(TimerHandle, this, &UWilloWispSpawning::OnSpawnTimerElapsed, CoolDownInSeconds, false);
		WilloWispState = WilloWispStatus::Spawning;
	}
}

void UWilloWispSpawning::SpawnWilloWisps(FAttackParams SpawnWilloWispsAttackParam)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("WilloWisps Spawning In Spawning class"));
	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	ensure(World);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	for (int i = 0; i < NumberOfWilloWisps; i++)
	{
		TSubclassOf<AActor> WilloWispToSpawn = *WilloWispBluePrint;
		FVector SpawnLocation = SpawnWilloWispsAttackParam.Lantern->GetFireStartPos() + WilloWispsOffSets[i];
		FRotator SpawnRotation = FRotator::ZeroRotator;
		AWilloWisp* NewWilloWisp = World->SpawnActor<AWilloWisp>(WilloWispToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
		if (NewWilloWisp)
		{
			SpawnedWilloWisps->Add(NewWilloWisp);
			NewWilloWisp->AttachToActor(SpawnWilloWispsAttackParam.Lantern, FAttachmentTransformRules::KeepWorldTransform);
			NewWilloWisp->EnergyLevel = WilloWispEnergyIndividual;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(21, 5.0f, FColor::Red, TEXT("Failed to spawn WilloWisp"));
		}
	}
	HasSpawned = true;
	WilloWispState = WilloWispStatus::Spawned;
}

void UWilloWispSpawning::OnSpawnTimerElapsed()
{
	if (AttackParams.Lantern) 
	{
		SpawnWilloWisps(AttackParams);
	}
}
