// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PlayerAnimationController.h"

void UPlayerAnimationController::SetMovementState(int MovState)
{
	MovementState = MovState;
	//UE_LOG(LogTemp, Warning, TEXT("Movement State: %d"), MovementState);
}

void UPlayerAnimationController::SetAttackState(int AttState)
{
	AttackState = AttState;
	UE_LOG(LogTemp, Warning, TEXT("Attack State: %d"), AttackState);
}

void UPlayerAnimationController::SetAim(bool IsAiming)
{
	Aim = IsAiming;
	UE_LOG(LogTemp, Warning, TEXT("Aim: %d"), Aim);
}

