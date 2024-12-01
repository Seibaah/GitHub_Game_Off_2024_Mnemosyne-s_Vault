// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UI/AShortStayInLibraryHUD.h"
#include "Stat.generated.h"

/**
 * 
 */
UCLASS()
class ASHORTSTAYINLIBRARY_API UStat : public UObject
{
	GENERATED_BODY()

protected:	
	float CurrentValue = 0.f;
	float MaxValue = 0.f;
	UAShortStayInLibraryHUD* HUD = nullptr;

public:
	virtual float GetValue();
	virtual void SetValue(float);
	virtual void ChangeValueByOffset(float);
	virtual void SetMax(float);
	void SetHUD(UAShortStayInLibraryHUD* PlayerHUD);
};
