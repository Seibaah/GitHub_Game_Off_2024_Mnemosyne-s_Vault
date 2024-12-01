// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnergySource.generated.h"

class UPointLightComponent;
class UBoxComponent;

UCLASS()
class ASHORTSTAYINLIBRARY_API AEnergySource : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnergySource();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerManager* TimerManager;
	FTimerHandle TimerHandle; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	float MaxEnergy = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	float CurrentEnergy = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
	float IntensityMultiplier = 10.f; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
	float TimeTakesToFullyRechargeInSeconds = 10.f;
	bool IsGainingEnergy = false; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
	UPointLightComponent* PointLightComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	UBoxComponent* BoxCollisionComponent;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float LoseEnergy();
};
