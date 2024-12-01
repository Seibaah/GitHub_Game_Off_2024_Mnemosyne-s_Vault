// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Lantern.h"

// Sets default values
ALantern::ALantern()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LanternMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lantern Mesh Component"));
	SetRootComponent(LanternMesh);

	LaserSocket = CreateDefaultSubobject<USceneComponent>(TEXT("Laser Socket Component"));
	LaserSocket->SetupAttachment(LanternMesh);
}

// Called when the game starts or when spawned
void ALantern::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<UActorComponent*> Meshes = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* ComponentPtr : Meshes) 
	{
		if (ComponentPtr->GetName() == "Laser") 
		{
			LaserMesh = Cast<UStaticMeshComponent>(ComponentPtr);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Got laser mesh"));
			ensure(LaserMesh);
			LaserMesh->SetVisibility(false);
		}
	}
}

// Called every frame
void ALantern::Tick(float DeltaTime)
{
	/*if (PlayerController) 
	{
		FRotator ControllerRotation = (*PlayerController)->GetControlRotation();
		FVector ForwardDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
		LaserSocket->SetWorldRotation(ForwardDirection.Rotation());
	}*/
}

UStaticMeshComponent* ALantern::GetMesh()
{
	return LanternMesh;
}

FVector ALantern::GetFireStartPos()
{
	return LaserSocket->GetComponentLocation();
}

void ALantern::SetLaserVisibility(bool IsVisible)
{
	LaserMesh->SetVisibility(IsVisible);
}
