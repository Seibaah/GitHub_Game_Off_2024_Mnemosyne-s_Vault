// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attacks/Attack.h"

UAttack::UAttack()
{
}

UAttack::UAttack(FString Name, int Damage, float CoolDownInSeconds, int EnergyNeeded) : Name(Name), Damage(Damage), CoolDownInSeconds(CoolDownInSeconds), EnergyNeeded(EnergyNeeded)
{
}

UAttack::~UAttack()
{
}

void UAttack::StartAttack(FAttackParams AttackParams)
{
}

void UAttack::EndAttack()
{
}
