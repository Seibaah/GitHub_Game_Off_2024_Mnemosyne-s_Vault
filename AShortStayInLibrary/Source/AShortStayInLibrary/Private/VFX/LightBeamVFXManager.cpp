// Fill out your copyright notice in the Description page of Project Settings.


#include "VFX/LightBeamVFXManager.h"

// Sets default values
ALightBeamVFXManager::ALightBeamVFXManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Add our Interaction Delegate
	BeamDelegate.AddDynamic(this, &ALightBeamVFXManager::InteractWithBeam);
}

// Called when the game starts or when spawned
void ALightBeamVFXManager::BeginPlay()
{
	Super::BeginPlay();
}

void ALightBeamVFXManager::SetStartPosition(FVector NewStartPosition)
{
	StartPosition = NewStartPosition;
}

void ALightBeamVFXManager::InteractWithBeam()
{
	//Print beam length, start position, and end position
	UE_LOG(LogTemp, Warning, TEXT("Beam Length: %f"), BeamLength);
	UE_LOG(LogTemp, Warning, TEXT("Start Position: %s"), *GetStartPosition().ToString());
	UE_LOG(LogTemp, Warning, TEXT("End Position: %s"), *GetEndPosition().ToString());
}


void ALightBeamVFXManager::SetEndPosition(FVector NewEndPosition)
{
	EndPosition = NewEndPosition;
}

void ALightBeamVFXManager::SetBeamLength(float NewBeamLength)
{
	BeamLength = NewBeamLength;
}

// Called every frame
void ALightBeamVFXManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

