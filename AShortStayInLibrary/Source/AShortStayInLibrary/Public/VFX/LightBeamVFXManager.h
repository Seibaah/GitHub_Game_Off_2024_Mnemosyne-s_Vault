// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightBeamVFXManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBeamDelegate);

UCLASS()
class ASHORTSTAYINLIBRARY_API ALightBeamVFXManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightBeamVFXManager();
	UFUNCTION(BlueprintCallable, Category = "VFX")
	void SetBeamLength(float NewBeamLength);
	UFUNCTION(BlueprintCallable, Category = "VFX")

	void SetStartPosition(FVector StartPosition);
	UFUNCTION(BlueprintCallable, Category = "VFX")

	void InteractWithBeam();	
	UFUNCTION(BlueprintCallable, Category = "VFX")
	void SetEndPosition(FVector StartPosition);

	float GetBeamLength() { return BeamLength; }
	FVector GetStartPosition() { return StartPosition; }
	FVector GetEndPosition() { return EndPosition; }


	UPROPERTY(BlueprintCallable, Category = "VFX")
	FBeamDelegate BeamDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float BeamLength;
	FVector StartPosition;
	FVector EndPosition;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
