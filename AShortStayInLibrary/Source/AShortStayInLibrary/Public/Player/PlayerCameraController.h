// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "PlayerCameraController.generated.h"

class FTimerManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASHORTSTAYINLIBRARY_API UPlayerCameraController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerCameraController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void TranslateCamera(FVector Start, FVector End);

	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Camera Position")
	FVector NonAimingPosition = FVector(0.f, 50.f, 50.f);
	UPROPERTY(EditAnywhere, Category = "Camera Position")
	FVector AimingPosition = FVector(100.f, 0.f, 50.f);

	bool IsTransitioning = false; 

	FVector StartPos;
	FVector EndPos;
	FTimerManager* TimerManager;
	FTimerHandle TimerHandle;
	UPROPERTY(EditAnywhere, Category = "Timer")
	float TransitionSpeedCMS = 700.f;
	float TimerDuration;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetUpCamera(USpringArmComponent* SpringArmComponent, UCameraComponent* CameraComponent);
	void StartAiming();
	void StopAiming();
};
