// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/MeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "Lantern.generated.h"

UCLASS()
class ASHORTSTAYINLIBRARY_API ALantern : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALantern();

	friend class AMainCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Lantern")
	UStaticMeshComponent* LanternMesh;
	UStaticMeshComponent* LaserMesh;

	UPROPERTY(EditAnywhere, Category = "Laser")
	USceneComponent* LaserSocket;
	//UNiagaraComponent* LaserVFX;

	TObjectPtr<AController>* PlayerController; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UStaticMeshComponent* GetMesh();
	FVector GetFireStartPos();
	void SetLaserVisibility(bool IsVisible);
};
