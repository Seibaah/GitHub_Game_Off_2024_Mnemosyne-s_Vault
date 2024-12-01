// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Enemy/Enemy.h"
#include "Components/PointLightComponent.h"
#include "WilloWisp.generated.h"

UCLASS()
class ASHORTSTAYINLIBRARY_API AWilloWisp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	enum class WilloWispState
	{
		Idle, Active, ChasingEnemy
	};

	AWilloWisp();

	friend class UWilloWispSpawning;
	friend class UBookEffectsManager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void Move(float DeltaTime);
	void CalculateSpeedDamageDirectionLight(float DeltaTime);
	void Die();
	UFUNCTION()
	void OnTriggerEnterInner(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTriggerEnterOuter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* WilloWispMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	float EnergyLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	float FullEnergyLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	float Damage; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	float EnergyDissipateRate = 0.1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageMultiplier = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
	UPointLightComponent* SpotLight = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
	float LightIntensityMultiplier = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int WilloWispSpeedMultiplier = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector MovementDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float LerpSpeed = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AimingDirectionOffset = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector AimingDirecion;

	UPROPERTY(EditAnywhere, Category = "Trigger")
	USphereComponent* InnerTriggerComponent;
	UPROPERTY(EditAnywhere, Category = "Trigger")
	float InnerTriggerRadiusInCM = 100.f;

	UPROPERTY(EditAnywhere, Category = "Trigger")
	USphereComponent* OuterTriggerComponent;
	UPROPERTY(EditAnywhere, Category = "Trigger")
	float OuterTriggerRadiusInCM = 500.f;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	AActor* EnemyBeingChased;

	WilloWispState State = WilloWispState::Idle;

	FTimerHandle TimerHandle;
	FTimerManager* TimerManager;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Fire(FVector Direction, const FVector& LanternPos);
};
