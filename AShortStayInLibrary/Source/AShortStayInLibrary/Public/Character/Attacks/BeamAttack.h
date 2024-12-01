// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Character/Attacks/Attack.h"
//#include "Player/MainCharacter.h"
#include "BeamAttack.generated.h"

/**
 * 
 */
UCLASS()
class ASHORTSTAYINLIBRARY_API UBeamAttack :public UAttack
{
	GENERATED_BODY()
	
public:
	UBeamAttack();
	UBeamAttack(FString name, int Damage, float CoolDownInSeconds, int EnergyNeeded, float BeamDistanceInCM);
	
	friend class UPlayerAttackManger;
	friend class AMainCharacter;

	void StartAttack(FAttackParams FAttackParams) override;
	void SetUp(FString BeamAttackName, int BeamAttackDamage, float BeamAttackCoolDownInSeconds, int BeamAttackEnergyNeeded, float BeamAttackDistanceInCM, TSubclassOf<AActor>* beamVFXBP);
protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	float BeamDistanceInCM;
	TSubclassOf<AActor>* BeamVFXBlueprint; 
};
