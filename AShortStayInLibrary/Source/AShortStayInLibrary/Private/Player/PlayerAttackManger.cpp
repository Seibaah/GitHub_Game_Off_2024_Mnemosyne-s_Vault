// Fill out your copyright notice in the Description page of Project Settings.
#include "Player/PlayerAttackManger.h"

// Sets default values for this component's properties
UPlayerAttackManger::UPlayerAttackManger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

UPlayerAttackManger::~UPlayerAttackManger()
{
}

// Called when the game starts
void UPlayerAttackManger::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		TimerManager = &World->GetTimerManager();
	}

	UAttack* BAttack = NewObject<UBeamAttack>(this, UBeamAttack::StaticClass());
	UBeamAttack* BeamAttack = Cast<UBeamAttack>(BAttack);
	BeamAttack->SetUp("Beam", BeamAttackDamage, BeamAttackCoolDownInSeconds, BeamAttackEnergy, BeamAttackDistanceInCM, &BeamAttackVFXBP);
	PlayerAttacksMap.Add("Beam", BAttack);
	FTimerHandle BeamHandle = FTimerHandle();
	PlayerAttackTimersMap.Add("Beam", BeamHandle);
	BeamAttack->TimerHandleForCoolDown = &BeamHandle;

	UAttack* WilloWispSpawningAttack = NewObject<UWilloWispSpawning>(this, UWilloWispSpawning::StaticClass());
	UWilloWispSpawning* WilloWispSpawning = Cast<UWilloWispSpawning>(WilloWispSpawningAttack);
	WilloWispSpawning->SetUp("WilloWisp Spawning", WilloWispEnergyForOne * WilloWispNumberPerSpawn, WilloWispChargingCoolDownInSeconds, WilloWispNumberPerSpawn, WilloWispOffsetsInCm, &SpawnedWilloWisps, TimerManager, &WilloWispBluePrint, WilloWispEnergyForOne);
	PlayerAttacksMap.Add("WilloWisp Spawning", WilloWispSpawningAttack);
	FTimerHandle WilloWispSpawnHandle = FTimerHandle();
	PlayerAttackTimersMap.Add("WilloWisp Spawning", WilloWispSpawnHandle);
	//WilloWispSpawningAttack->TimerHandleForCoolDown = &WilloWispSpawnHandle;

	UAttack* WilloWispSendingAttack = NewObject<UWilloWispSending>(this, UWilloWispSending::StaticClass());
	UWilloWispSending* WilloWispSending = Cast<UWilloWispSending>(WilloWispSendingAttack);
	WilloWispSending->SetUp(PlayerAttackTimersMap.Find("WilloWisp Spawning"), TimerManager, &SpawnedWilloWisps, WilloWispSpawningCoolDownInSeconds);
	PlayerAttacksMap.Add("WilloWisp Sending", WilloWispSendingAttack);
	FTimerHandle WilloWispSendHandle = FTimerHandle();
	PlayerAttackTimersMap.Add("WilloWisp Sending", WilloWispSendHandle);
	//WilloWispSendingAttack->TimerHandleForCoolDown = &WilloWispSendHandle;

	UAttack* UltimateAttack = NewObject<UUltimateAttack>(this, UUltimateAttack::StaticClass());
	Ultimate = Cast<UUltimateAttack>(UltimateAttack);
	Ultimate->SetUp(UltimateBeamRadius, UltimateCoolDownInSeconds, UltimateEnergyNeeded, &UltimateVFXBP, UltimateDamage, UltimateDespawnTime, TimerManager, MainCharacterController);
	PlayerAttacksMap.Add("Ultimate", UltimateAttack);
	FTimerHandle UltimateTimerHandle = FTimerHandle(); 
	PlayerAttackTimersMap.Add("Ultimate", UltimateTimerHandle);
	//UltimateAttack->TimerHandleForCoolDown = &UltimateTimerHandle;
}

// Called every frame
void UPlayerAttackManger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Ultimate->MainCharacterController) 
	{
		Ultimate->MainCharacterController = MainCharacterController;
	}
	if (!Ultimate->AnimController && AnimController) 
	{
		Ultimate->AnimController = AnimController;
	}
	if (IsUltimateInCoolDown) 
	{
		FTimerHandle* handle = PlayerAttackTimersMap.Find("Ultimate");
		if (TimerManager->IsTimerActive(*handle)) 
		{
			ensure(HUD);
			HUD->SetUltimateCoolDown(TimerManager->GetTimerElapsed(*handle) / UltimateCoolDownInSeconds);
		}
		else
		{
			IsUltimateInCoolDown = false;
		}
	}
	if (IsBeamInCoolDown) 
	{
		FTimerHandle* handle = PlayerAttackTimersMap.Find("Beam");
		if (TimerManager->IsTimerActive(*handle))
		{
			ensure(HUD);
			HUD->SetBeamCoolDown(TimerManager->GetTimerElapsed(*handle) / BeamAttackCoolDownInSeconds);
		}
		else
		{
			IsBeamInCoolDown = false;
		}
	}
	FTimerHandle* WilloWispSpawnHandle = PlayerAttackTimersMap.Find("WilloWisp Spawning");
	if (HUD && SpawnedWilloWisps.Num() == 0 && TimerManager->IsTimerActive(*WilloWispSpawnHandle))
	{
		HUD->SetWilloWispCoolDown(TimerManager->GetTimerElapsed(*WilloWispSpawnHandle) / 1.f);
	}
}

void UPlayerAttackManger::Attack(FString AttackName, UEnergy* PlayerEnergy, FAttackParams AttackParams)
{
	if (!IsAiming) return;
	UAttack* Attack = *PlayerAttacksMap.Find(AttackName);
	ensure(Attack);
	FTimerHandle* CoolDownTimer = PlayerAttackTimersMap.Find(AttackName);
	ensure(CoolDownTimer);
	//willowisps has no automatic cool down after spawning but it can have a cool down after all willowisps have been sent out.
	if (!TimerManager->IsTimerActive(*CoolDownTimer) && !TimerManager->IsTimerPaused(*CoolDownTimer))
	{
		if (PlayerEnergy->GetValue() < Attack->EnergyNeeded) return;
		PlayerEnergy->ChangeValueByOffset(-Attack->EnergyNeeded);
		GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Red, FString::Printf(TEXT("Current Player Energy: %f"), PlayerEnergy->GetValue()));
		TimerManager->SetTimer(*CoolDownTimer, this, &UPlayerAttackManger::OnAttackingTimerElapsed, Attack->CoolDownInSeconds, false);
		Attack->StartAttack(AttackParams);
		if (AttackName == "WilloWisp Sending") 
		{
			if (AnimController)
			{
				AnimController->SetAttackState((int)AttackState::WilloWispSending);
			}
		}
		else if (AttackName == "WilloWisp Spawning")
		{
			if (AnimController)
			{
				AnimController->SetAttackState((int)AttackState::WilloWispSpawning);
			}
		}
		else if (AttackName == "Ultimate") 
		{
			if (AnimController)
			{
				AnimController->SetAttackState((int)AttackState::UltimateCharge);
				IsUltimateInCoolDown = true;
			}
		}
		else if (AttackName == "Beam")
		{
			if (AnimController)
			{
				AnimController->SetAttackState((int)AttackState::Beam);
				IsBeamInCoolDown = true;
			}
		}
	}
}

void UPlayerAttackManger::OnAttackingTimerElapsed()
{
}

UAttack* UPlayerAttackManger::GetAttackInstanceByName(FString Name)
{
	UAttack* Attack = *PlayerAttacksMap.Find(Name);
	return Attack;
}

void UPlayerAttackManger::AimStatusChange(bool Status)
{
	IsAiming = Status;
}

bool UPlayerAttackManger::IfHaveWilloWisps()
{
	return !SpawnedWilloWisps.IsEmpty();
}

void UPlayerAttackManger::SetSpawnedWilloWisps(TArray<AWilloWisp*>* WilloWisps)
{
	SpawnedWilloWisps = *WilloWisps;
}

TArray<AWilloWisp*>* UPlayerAttackManger::GetSpawnedWilloWisps()
{
	return &SpawnedWilloWisps;
}