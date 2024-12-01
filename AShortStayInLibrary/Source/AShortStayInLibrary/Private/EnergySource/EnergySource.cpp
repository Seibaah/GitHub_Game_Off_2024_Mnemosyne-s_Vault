// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergySource/EnergySource.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnergySource::AEnergySource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
	SetRootComponent(BoxCollisionComponent);

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light Component"));
	PointLightComponent->SetupAttachment(RootComponent);
	PointLightComponent->SetIntensity(CurrentEnergy * IntensityMultiplier);
}

void AEnergySource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsGainingEnergy) 
	{
		if (!TimerManager->IsTimerActive(TimerHandle)) 
		{	
			IsGainingEnergy = false;
			return;
		}
		float TimePassed = TimerManager->GetTimerElapsed(TimerHandle);
		CurrentEnergy = MaxEnergy * (TimePassed / TimeTakesToFullyRechargeInSeconds);
		if (PointLightComponent)
		{
			PointLightComponent->SetIntensity(CurrentEnergy * IntensityMultiplier);
		}
	}
}


// Called when the game starts or when spawned
void AEnergySource::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	if (World)
	{
		TimerManager = &World->GetTimerManager();
	}
}

float AEnergySource::LoseEnergy()
{
	float Energy = CurrentEnergy;
	CurrentEnergy = 0;
	TimerManager->SetTimer(TimerHandle, TimeTakesToFullyRechargeInSeconds, false);
	IsGainingEnergy = true;
	return Energy;
}
