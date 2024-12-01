// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "FloatingBook.generated.h"

/** Enum to define different book behaviors */
UENUM(BlueprintType)
enum class EBookBehavior : uint8
{
	Interactable UMETA(DisplayName = "Interactable"),
	Repellent UMETA(DisplayName = "Repellent"),
	Follower UMETA(DisplayName = "Follower")
};

class UBookEffectsManager; 
class UAShortStayInLibraryHUD;

UCLASS()
class ASHORTSTAYINLIBRARY_API AFloatingBook : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingBook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//-----PUBLIC FUNCTIONS-----
	void RotateTowardsTarget(FVector Target);
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetBookCollider(UShapeComponent* collider);

	UFUNCTION(BlueprintCallable, Category = "Book Physics") 
	void SetTargetActor(USceneComponent* Target);
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetPageBookAnimInstances(TArray<UAnimInstance*> Instances);
	UFUNCTION(BlueprintCallable, Category = "Book Physics")
	UAnimInstance* GetLeftPage();

	UFUNCTION(BlueprintCallable, Category = "Book Physics")
	UAnimInstance* GetRightPage();

	UFUNCTION(BlueprintCallable, Category = "Book Effects")
	void SetupAndApplyEffect();
	UFUNCTION(BlueprintCallable, Category = "Book Effects")
	void SwitchActivePages(bool DisableLeftPage, bool OnOpenBook);
	UFUNCTION(BlueprintCallable, Category = "Book Effects")
	void SetReadBook();
	UFUNCTION(BlueprintCallable, Category = "Book Effects")
	UTexture2D* GetTexture();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetUITextVisibility(bool ShouldShow);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void PauseTime(bool ShouldPauseTime);

	void SetBookID(int);
	int GetBookID();
	void SetHUD(UAShortStayInLibraryHUD*);

	void SetBookEffectsManager(UBookEffectsManager*);


	//-------COMPONENTS---------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Book Physics")
	USceneComponent* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	UShapeComponent* BookCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	bool bGoTowardsObjective;

	// Array to hold instances of PageBookAnim
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	TArray<UAnimInstance*> PageBookAnimInstances;


private:

	///-----EDITOR VARIABLES-----
	UPROPERTY(EditAnywhere, Category = "Book Physics")
	float Amplitude = 0.3f;  // Controls how high the book floats

	UPROPERTY(EditAnywhere, Category = "Book Physics")
	float Frequency = 1.0f;  // Controls how fast the book floats

	UPROPERTY(EditAnywhere, Category = "Book Physics", meta=(ClampMin = "0.0", ClampMax = "10.0"))
	//The book rotation speed
	float RotationSpeed = 2.0f;

	UPROPERTY(Category = "Book Physics", EditAnywhere)
	EBookBehavior BookBehavior = EBookBehavior::Interactable;

	UPROPERTY(EditAnywhere, Category = "Book Physics")
	float RepelRadius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Book Physics")
	float ForceFieldStrength = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Book Physics")
	float AttractRadius = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Book Physics")
	float InteractRadius = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Book Physics", meta=(ClampMin = "0.01", ClampMax = "0.1"))
	float DampeningFactor = 0.03f;

	UPROPERTY(EditDefaultsOnly, Category = "Book Camera Settings")
	USceneComponent* CameraTransform;
	
	UPROPERTY(EditAnywhere, Category = "Book ID")
	int BookID;

	UPROPERTY(EditAnywhere, Category = "Book read status")
	bool IsRead = false;

	//-----PRIVATE VARIABLES-----s
	FVector StartLocation;
	float RunningTime;
	FVector NewLocation;
	float DeltaZ;
	bool bLockBookInPlace = false;
	bool bLookAtTarget = true;
	//x is left page and y is right page
	FVector2D BookDoubleIndex;
	UBookEffectsManager* BookEffectsManager = nullptr;
	UAShortStayInLibraryHUD* HUD = nullptr;
	int YBufferIndex = 0;
	int XBufferIndex = 0;
	UTexture2D* Texture = nullptr;
	

	//-----PRIVATE FUNCTIONS-----
	void RestoreToHoverPosition();
	void HandleTargetInteraction();
	void HandleRepellentBehavior(float Distance);
	void HandleFollowerBehavior(float Distance);
	void HandleInteractableBehavior(float Distance);
};


