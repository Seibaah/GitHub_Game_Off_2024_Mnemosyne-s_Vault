// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCameraController.h"

// Sets default values for this component's properties
UPlayerCameraController::UPlayerCameraController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerCameraController::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	ensure(World);
	TimerManager = &World->GetTimerManager();
	ensure(TimerManager);
	TimerHandle = FTimerHandle();
}

// Called every frame
void UPlayerCameraController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsTransitioning) 
	{
		float TimeElapsed = TimerManager->GetTimerElapsed(TimerHandle);
		float Ratio = FMath::Clamp(TimeElapsed / TimerDuration, 0.0f, 1.0f);
		GEngine->AddOnScreenDebugMessage(16, 1.f, FColor::Red, FString::Printf(TEXT("Aiming Timer ratio: %f"), Ratio));
		SpringArm->SetRelativeLocation((1 - Ratio) * StartPos + Ratio * EndPos);
		if (!TimerManager->IsTimerActive(TimerHandle))
		{
			IsTransitioning = false;
			SpringArm->SetRelativeLocation(EndPos);
		}
	}
}

void UPlayerCameraController::SetUpCamera(USpringArmComponent* SpringArmComponent, UCameraComponent* CameraComponent)
{
	this->SpringArm = SpringArmComponent;
	this->Camera = CameraComponent;
}

void UPlayerCameraController::StartAiming()
{
	FVector StartPosition = SpringArm->GetRelativeTransform().GetLocation(); 
	FVector EndPosition = AimingPosition;
	TranslateCamera(StartPosition, EndPosition);
}

void UPlayerCameraController::StopAiming()
{
	FVector StartPosition = SpringArm->GetRelativeTransform().GetLocation();
	FVector EndPosition = NonAimingPosition;
	TranslateCamera(StartPosition, EndPosition);
}

void UPlayerCameraController::TranslateCamera(FVector Start, FVector End)
{
	ensure(TimerManager);
	float DistanceInCM = FVector::Distance(Start, End);
	GEngine->AddOnScreenDebugMessage(17, 1.f, FColor::Magenta, FString::Printf(TEXT("Distance: %f"), DistanceInCM));
	TimerDuration = DistanceInCM / TransitionSpeedCMS;
	GEngine->AddOnScreenDebugMessage(15, 1.f, FColor::Magenta, FString::Printf(TEXT("Aiming Timer Duration: %f"), TimerDuration));
	TimerManager->SetTimer(TimerHandle, TimerDuration, false);
	StartPos = Start;
	EndPos = End;
	IsTransitioning = true;
}


