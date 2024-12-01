// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Energy.h"

void UEnergy::SetValue(float Val)
{
	Super::SetValue(Val);

	if (HUD)
	{
		HUD->SetEnergy(Val, MaxValue);
	}
}

void UEnergy::ChangeValueByOffset(float Val)
{
	Super::ChangeValueByOffset(Val);

	if (HUD)
	{
		HUD->SetEnergy(CurrentValue, MaxValue);
	}
}

void UEnergy::SetMax(float Max)
{
	Super::SetMax(Max);

	if (HUD)
	{
		HUD->SetEnergy(CurrentValue, Max);
	}
}
