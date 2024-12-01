// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Stat.h"

float UStat::GetValue()
{
	return CurrentValue;
}

void UStat::SetValue(float Val)
{
	CurrentValue = Val;
	CurrentValue = std::min(CurrentValue, MaxValue);
}

void UStat::ChangeValueByOffset(float Offset)
{
	CurrentValue += Offset;
	CurrentValue = std::min(CurrentValue, MaxValue);
}

void UStat::SetMax(float Max)
{
	MaxValue = Max;
}

void UStat::SetHUD(UAShortStayInLibraryHUD* PlayerHUD)
{
	HUD = PlayerHUD;
}
