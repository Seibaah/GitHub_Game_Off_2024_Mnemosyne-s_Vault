// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BookEffectsManager.generated.h"

class AMainCharacter;
class UPlayerAttackManger;
class AWilloWisp;
class AFloatingBook;
class UAShortStayInLibraryHUD;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASHORTSTAYINLIBRARY_API UBookEffectsManager : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	using BookEffectFunction = void(UBookEffectsManager::*)();

	UBookEffectsManager();

	void MaxHPUpgrade();
	void HPUpgrade();

	void MaxEnergyUpgrade();
	void WilloWispEnergyDissipateRateUpgrade();
	void AttacksDamageUpgrade();
	void LoreBookCollect();

	void SetUpBook();

	friend class AMainCharacter;
	friend class AFloatingBook;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetUpFunctions();
	void SetUpIDPool();
	void SetUpBookContent();
	//void SetUpTextures();

	AMainCharacter* MainCharacter = nullptr;
	UPlayerAttackManger* PlayerAttackManager = nullptr;
	TSubclassOf<AActor>* WilloWispBP; 
	TArray<BookEffectFunction> BookEffects;
	TArray<int> BookIDPool;
	TMap<int, FString> BookContent;
	UAShortStayInLibraryHUD* HUD = nullptr;
	bool AreBooksSetUp = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Books")
	TArray<AFloatingBook*> Books; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Books")
	TArray<UTexture2D*> Textures;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades")
	float MaxHPUpgradeAmount = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades")
	float HPUpgradeAmount = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades")
	float MaxEnergyUpgradeAmount = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades")
	float PlayerAttacksDamageUpgradePercentage = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades")
	float WilloWispEnergyDissipateRateUpgradePercentage = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lore")
	int LoreBookCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lore")
	int LoreBookCountTotal = 5;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
