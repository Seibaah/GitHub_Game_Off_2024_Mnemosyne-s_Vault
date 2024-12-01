// Fill out your copyright notice in the Description page of Project Settings.


#include "Book/BookEffectsManager.h"
#include "Player/MainCharacter.h"
#include "Player/PlayerAttackManger.h"
#include "Character/Attacks/WilloWisp.h"
#include "Book/FloatingBook.h"
#include "UI/AShortStayInLibraryHUD.h"
#include "Character/Health.h"

// Sets default values for this component's properties
UBookEffectsManager::UBookEffectsManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UBookEffectsManager::BeginPlay()
{
	Super::BeginPlay();

	SetUpFunctions();
	SetUpIDPool();
	
}

// Called every frame
void UBookEffectsManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (HUD && !AreBooksSetUp)
	{
		SetUpBook();
		AreBooksSetUp = true;
	}
}

void UBookEffectsManager::MaxHPUpgrade()
{
	if (MainCharacter) 
	{
		UHealth* Health = MainCharacter->GetHealth();
		Health->SetMax(MainCharacter->MaxHealth + MaxHPUpgradeAmount);
		Health->HealFull();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("max hp upgraded"));
	}
}

void UBookEffectsManager::HPUpgrade()
{
	if (MainCharacter) 
	{
		MainCharacter->GetHealth()->ChangeValueByOffset(HPUpgradeAmount);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("healed"));
	}
}

void UBookEffectsManager::MaxEnergyUpgrade()
{
	if (MainCharacter)
	{
		MainCharacter->Energy->SetMax(MaxEnergyUpgradeAmount);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Max energy upgraded"));
	}
}

void UBookEffectsManager::WilloWispEnergyDissipateRateUpgrade()
{
	AActor* CDO = WilloWispBP->GetDefaultObject();
	AWilloWisp* CDOWilloWisp = Cast<AWilloWisp>(CDO);
	CDOWilloWisp->EnergyDissipateRate *= WilloWispEnergyDissipateRateUpgradePercentage;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("willowisp upgraded"));
}

void UBookEffectsManager::AttacksDamageUpgrade()
{
	ensure(PlayerAttackManager);
	for (TPair<FString, UAttack*>& pair : PlayerAttackManager->PlayerAttacksMap)
	{
		UAttack* Attack = pair.Value;
		Attack->Damage *= (1 + PlayerAttacksDamageUpgradePercentage);
	}
	AActor* CDO = WilloWispBP->GetDefaultObject();
	AWilloWisp* CDOWilloWisp = Cast<AWilloWisp>(CDO);
	CDOWilloWisp->DamageMultiplier *= (1 + PlayerAttacksDamageUpgradePercentage);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("attack damage upgraded"));
}

void UBookEffectsManager::LoreBookCollect()
{
	LoreBookCount++;
	if (LoreBookCount == LoreBookCountTotal) 
	{
		if (HUD) 
		{
			HUD->ChangeVisibilityWinningScreen(true);
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Method called"));
}

void UBookEffectsManager::SetUpBook()
{
	for (AFloatingBook* book : Books) 
	{
		ensure(BookIDPool.Num() != 0);
		int id = BookIDPool.Last();
		BookIDPool.Pop();
		book->SetBookID(id); 
		book->SetBookEffectsManager(this);
		book->SetHUD(HUD);
	}
}

void UBookEffectsManager::SetUpFunctions()
{
	BookEffects.Add(&UBookEffectsManager::MaxHPUpgrade);
	BookEffects.Add(&UBookEffectsManager::HPUpgrade);
	BookEffects.Add(&UBookEffectsManager::MaxEnergyUpgrade);
	BookEffects.Add(&UBookEffectsManager::WilloWispEnergyDissipateRateUpgrade);
	BookEffects.Add(&UBookEffectsManager::AttacksDamageUpgrade);
	for (int i = 0; i < 5; i++) 
	{
		BookEffects.Add(&UBookEffectsManager::LoreBookCollect);
	}
}

void UBookEffectsManager::SetUpIDPool()
{
	for (int i = 0; i < 10; i++) 
	{
		BookIDPool.Add(i);
	}
	auto cmp = [](const int Item1, const int Item2) {return FMath::FRand() < 0.5f; };
	BookIDPool.Sort(cmp);
}

void UBookEffectsManager::SetUpBookContent()
{
	BookContent.Add(0, FString::Printf(TEXT("Max health increase by %.0f"), HPUpgradeAmount));
	BookContent.Add(1, FString::Printf(TEXT("Health increase by %.0f"), HPUpgradeAmount));
	BookContent.Add(2, FString::Printf(TEXT("Max light increase by %.0f"), MaxEnergyUpgradeAmount));
	BookContent.Add(3, FString::Printf(TEXT("WilloWisp light Dissipation Rate decrease by %.0f percent"), WilloWispEnergyDissipateRateUpgradePercentage * 100));
	BookContent.Add(4, FString::Printf(TEXT("Player damage increase by %.0f percent"), PlayerAttacksDamageUpgradePercentage * 100));
	
	BookContent.Add(5, FString::Printf(TEXT(
		"When the Blade of Truth struck the Sun, life began to flourish on Earth."
		"Flowers blossomed, trees sprouted, gardens flourished, and animals found an abundance of food."
		"As the Blade of Truth pierced the Sun, it’s handle fell from the cosmos and landed on Earth."
		"This handle became known as the Rod of Power.Whosoever possesses the Rod shall reign over Earth. \n\n"
		"“What lurks in the shadows will show you the way.”\n"
		"- Mnemosyne"
	)));
	BookContent.Add(6, FString(TEXT(
		"The Tree Of Knowledge only grants those that are worthy.\n\n"
		"“Prove to me you are worthy. Overcome your memory loss and you will be granted power beyond your wildest dreams.”\n"
		"- Mnemosyne"
	)));

	BookContent.Add(7, FString(TEXT(
		"This library is the key to the universal consciousness. This book is “The Spirit Of The Dead.”\n\n"
		"“When the dead come knocking, open the door.”\n"
		"- Mnemosyne"
	)));

	BookContent.Add(8, FString(TEXT(
		"The world was built in harmony, but man has spilled too much blood, which has summoned the evil spirits. The pillars were built to withstand the suffering of humanity.\n\n"
		"“It is imperative that the Pillars Of Justice keep standing. If not, then the evil spirits in this library will be free of their prison.”\n"
		"- Mnemosyne"
	)));

	BookContent.Add(9, FString(TEXT(
		"You are the sun that shines the light, the beacon of hope, the wielder of knowledge, you are both dead and alive. Go forth with the Rod Of Power and follow your destiny. Use your strength to lift the all-powerful rod.\n\n"
		"“You are the chosen one. Take my hand.”\n"
		"- Mnemosyne"
	)));
}
