// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Player/PlayerAttackManger.h"
#include "Character/Attacks/Attack.h"
#include "Player/PlayerCameraController.h"
#include "Components/StaticMeshComponent.h"
#include "Player/Lantern.h"
#include "Components/SphereComponent.h"
#include "Character/Health.h"
#include "Character/Energy.h"
#include "UI/AShortStayInLibraryHUD.h"
#include "MainCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UCharacterMovementComponent;
class FTimerManager;
class UPlayerAnimationController;
class AEnergySource;

UCLASS(Blueprintable)
class ASHORTSTAYINLIBRARY_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public: 
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/*void UseEnergy(int EnergyUsed);*/
	
	MovementState GetCurrentMovementState();

	UFUNCTION(BlueprintPure, Category = "Health")
	UHealth* GetHealth();
	UFUNCTION(BlueprintCallable, Category = "Book")
	void SetIsReading(bool IsReadingBook);

	friend class UBookEffectsManager;

	//void ResetPlayerStatus();

protected:
	virtual void BeginPlay() override;

private:
	void Move(const FInputActionValue& Value);
	void StopMoving(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void StartAiming(const FInputActionValue& Value);
	void EndAiming(const FInputActionValue& Value);

	void BeamAttack(const FInputActionValue& Value);
	void WilloWispAttack(const FInputActionValue& Value);
	void UltimateAttack(const FInputActionValue& Value);

	void InteractWithEnergySource(const FInputActionValue& Value);
	void PauseGame(const FInputActionValue& Value);

	UFUNCTION()
	void OnEnterTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckJumpingState();

	APlayerController* MainCharacterController = nullptr; 
	int MainCharacterInputMappingConTextPriority = 0;
	int UIInputMappingContextPriority = 10;
	MovementState MovementState = MovementState::Idle;
	AttackState AttackState = AttackState::None;
	
protected:
	UCharacterMovementComponent* CharacterMovementComponent; 

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* CharacterContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* UIContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* BeamAttackAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AimAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* WilloWispAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* UltimateAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InteractEnergySourceAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* PauseAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	float JumpVelocity = 100.f; 

	UPROPERTY(EditAnywhere, Category = "Trigger")
	USphereComponent* SphereComponent; 
	UPROPERTY(EditAnywhere, Category = "Trigger")
	float SphereComponentRadius = 100.f; 

	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UPlayerCameraController* PlayerCameraController; 

	UPROPERTY(EditAnywhere, Category = "Attack")
	UPlayerAttackManger* PlayerAttackManager; 


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lantern")
	USceneComponent* LanternSocket; 
	UPROPERTY(EditAnywhere, Category = "Lantern")
	ALantern* Lantern;
	UPROPERTY(EditAnywhere, Category = "Lantern")
	bool IsLanternAttached = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lantern")
	TSubclassOf<AActor> LanternBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	UHealth* Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHealth = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	UEnergy* Energy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxEnergy = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UPlayerAnimationController* AnimController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh")
	USkeletalMeshComponent* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	UAShortStayInLibraryHUD* HUD = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnergySource")
	AEnergySource* EnergySource = nullptr; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Book")
	UBookEffectsManager* BookManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Book")
	bool IsReading = false;
};
