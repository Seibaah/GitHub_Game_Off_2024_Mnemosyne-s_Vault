// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Enemy/Enemy.h"
#include "Player/Lantern.h"
#include "Attack.generated.h"

/**
 * Attack base class, the actual attack classes will inherit this
 */

struct FAttackParams
{
	FVector ShootDirection;
	ALantern* Lantern;

	FAttackParams(ALantern* Lantern = nullptr, FVector ShootDirection = FVector::ZeroVector)
		:ShootDirection(ShootDirection), Lantern(Lantern)
	{}
};

UCLASS()
class ASHORTSTAYINLIBRARY_API UAttack : public UObject
{
	GENERATED_BODY()

public:
	enum class EAttacks 
	{
		WilloWisp, 
		Beam
	};

	UAttack();
	UAttack(FString name, int Damage, float CoolDownInSeconds, int EnergyNeeded);
	~UAttack();

	friend class UPlayerAttackManger;
	friend class AMainCharacter;
	friend class UBookEffectsManager;

	virtual void StartAttack(FAttackParams AttackParams);
	virtual void EndAttack();

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	FString Name;
	UPROPERTY(EditAnywhere, Category = "Attack")
	int Damage;
	UPROPERTY(EditAnywhere, Category = "Attack")
	int EnergyNeeded;
	UPROPERTY(EditAnywhere, Category = "Attack")
	float CoolDownInSeconds;
	FTimerHandle* TimerHandleForCoolDown; 
};
