// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class MovementState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Running UMETA(DisplayName = "Running"),
	Jumping UMETA(DisplayName = "Jumping"),
	Landing UMETA(DisplayName = "Landing")
};

// Declare AttackState as a UENUM outside the class
UENUM(BlueprintType)
enum class AttackState : uint8
{
	Beam UMETA(DisplayName = "Beam"),
	WilloWispSpawning UMETA(DisplayName = "WilloWispSpawning"),
	WilloWispSending UMETA(DisplayName = "WilloWispSending"),
	UltimateCharge UMETA(DisplayName = "UltimateCharge"),
	UltimateFire UMETA(DisplayName = "UltimateFire"),
	None UMETA(DisplayName = "None")
};
