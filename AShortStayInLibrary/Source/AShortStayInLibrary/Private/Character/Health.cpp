// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Health.h"

void UHealth::SetValue(float Val)
{
	Super::SetValue(Val);

	if (HUD) 
	{
		HUD->SetHealth(Val, MaxValue);
	}
}

void UHealth::ChangeValueByOffset(float Val)
{
	Super::ChangeValueByOffset(Val);
	
	if (HUD) 
	{
		if(CurrentValue <= 0)
		{
			HUD->ShowGameOverPanel();
		}
		HUD->SetHealth(CurrentValue, MaxValue);
	}
}

void UHealth::SetMax(float Max)
{
	Super::SetMax(Max);

	if (HUD)
	{
		HUD->SetHealth(CurrentValue, Max);
	}
}

void UHealth::HealFull()
{
	SetValue(MaxValue);
}
