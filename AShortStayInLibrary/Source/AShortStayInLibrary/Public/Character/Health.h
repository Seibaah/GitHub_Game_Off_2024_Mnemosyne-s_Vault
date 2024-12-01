// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Stat.h"
#include "Health.generated.h"

/**
 * 
 */
UCLASS()
class ASHORTSTAYINLIBRARY_API UHealth : public UStat
{
	GENERATED_BODY()
public:
	virtual void SetValue(float) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void ChangeValueByOffset(float Offset) override;
	
	virtual void SetMax(float) override;
	void HealFull();
};
