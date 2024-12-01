// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Attacks/Attack.h"
#include "Character/Attacks/BeamAttack.h"
#include "Character/Attacks/WilloWispSpawning.h"
#include "Character/Attacks/WilloWispSending.h"
#include "Character/Attacks/UltimateAttack.h"
#include "Player/PlayerMovementStates.h"
#include "Animation/PlayerAnimationController.h"
#include "Character/Energy.h"
#include "PlayerAttackManger.generated.h"

class UAttack;
class FTimerManager;
class AMainCharacter;
class APlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASHORTSTAYINLIBRARY_API UPlayerAttackManger : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerAttackManger();
	~UPlayerAttackManger();

	friend class AMainCharacter;
	friend class UBookEffectsManager;
		
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	TMap<FString, UAttack*> PlayerAttacksMap;
	TMap<FString, FTimerHandle> PlayerAttackTimersMap;
	FTimerManager* TimerManager; 
	APlayerController* MainCharacterController;
	UUltimateAttack* Ultimate;
	bool IsAiming = false;
	bool IsBeamInCoolDown = false;
	bool IsWilloWispInCoolDown = false;
	bool IsUltimateInCoolDown = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beam Attack")
	float BeamAttackEnergy = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beam Attack")
	int BeamAttackDamage = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beam Attack")
	float BeamAttackCoolDownInSeconds = 3.f; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beam Attack")
	float BeamAttackDistanceInCM = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beam Attack")
	TSubclassOf<AActor> BeamAttackVFXBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WilloWisp")
	float WilloWispEnergyForOne = 10000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WilloWisp")
	int WilloWispNumberPerSpawn = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WilloWisp")
	float WilloWispSpawningCoolDownInSeconds = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WilloWisp")
	float WilloWispChargingCoolDownInSeconds = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WilloWisp")
	TArray<FVector> WilloWispOffsetsInCm = { {10.f, 0.f, 15.f}, {0.f, 10.f, 15.f}, {0.f, -10.f, 15.f} };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WilloWisp")
	TSubclassOf<AActor> WilloWispBluePrint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WilloWisp")
	TArray<AWilloWisp*> SpawnedWilloWisps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
	float UltimateBeamRadius = 32.f; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
	float UltimateCoolDownInSeconds = 60.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
	float UltimateEnergyNeeded = 60.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
	TSubclassOf<ALightBeamVFXManager> UltimateVFXBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
	float UltimateDamage = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
	float UltimateDespawnTime = 10.f;
	UPlayerAnimationController* AnimController = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	UAShortStayInLibraryHUD* HUD = nullptr;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Attack(FString AttackType, UEnergy* PlayerEnergy, FAttackParams AttackParams);
	void OnAttackingTimerElapsed();
	UAttack* GetAttackInstanceByName(FString Name);
	void AimStatusChange(bool Status);	
	bool IfHaveWilloWisps();
	void SetSpawnedWilloWisps(TArray<AWilloWisp*>* WilloWisps);
	TArray<AWilloWisp*>* GetSpawnedWilloWisps();
};
