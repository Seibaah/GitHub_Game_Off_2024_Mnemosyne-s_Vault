// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Attacks/Attack.h"
#include "VFX/LightBeamVFXManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Animation/PlayerAnimationController.h"
#include "Player/PlayerMovementStates.h"
#include "UltimateAttack.generated.h"

/**
 * 
 */

UCLASS()
class ASHORTSTAYINLIBRARY_API UUltimateAttack : public UAttack
{
	GENERATED_BODY()
public:
	UUltimateAttack();
	~UUltimateAttack();

	friend class UPlayerAttackManger;
	friend class AMainCharacter;

	void SetUp(float BeamRadius, float UltimateCoolDownInSeconds, float Energy, TSubclassOf<ALightBeamVFXManager>* UltimateVFXBlueprint, float UltimateDamageInSeconds, float DesapwenTime, FTimerManager* Manager, APlayerController* MainCharacterController);
	void StartAttack(FAttackParams FAttackParams) override;
	UFUNCTION()
	void OnBeamFired(); 

protected:
	UFUNCTION()
	void DestryBeamVFX();
	FVector StartPos;
	FVector EndPos;
	FVector HalfSize;
	TSubclassOf<ALightBeamVFXManager>* LightBeamVFXManagerBlueprint;
	FRotator Orientation;
	UWorld* World = nullptr;
	ALantern* Lantern;
	float DespawnTimeInSeconds; 
	FTimerManager* TimerManager;
	FTimerHandle DespawnTimerHandle;
	ALightBeamVFXManager* CurrentVFX = nullptr;
	APlayerController* MainCharacterController = nullptr;
	UPlayerAnimationController* AnimController = nullptr;
};
