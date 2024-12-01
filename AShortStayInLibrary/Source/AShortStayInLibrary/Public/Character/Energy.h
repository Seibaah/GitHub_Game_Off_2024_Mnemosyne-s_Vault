// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Stat.h"
#include "Energy.generated.h"

/**
 * 
 */
UCLASS()
class ASHORTSTAYINLIBRARY_API UEnergy : public UStat
{
	GENERATED_BODY()
public:
	virtual void SetValue(float) override;
	virtual void ChangeValueByOffset(float) override;
	virtual void SetMax(float) override;
};
