// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimationController.generated.h"  
/**     
 * 
 */
UCLASS()
class ASHORTSTAYINLIBRARY_API UPlayerAnimationController : public UAnimInstance
{
	GENERATED_BODY()

public:
	void SetMovementState(int MovState);
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetAttackState(int AttState);
	void SetAim(bool IsAiming);


	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	int MovementState;
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	int AttackState = 5;
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool Aim;

}; 
