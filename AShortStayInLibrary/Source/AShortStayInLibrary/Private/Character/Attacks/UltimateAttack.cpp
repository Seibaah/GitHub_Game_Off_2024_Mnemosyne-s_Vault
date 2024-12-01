// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attacks/UltimateAttack.h"
#include "Enemy/EnemyHealthComponent.h"

UUltimateAttack::UUltimateAttack()
{
}

UUltimateAttack::~UUltimateAttack()
{
}

void UUltimateAttack::SetUp(float UltimateRadius, float CoolDown, float Energy, TSubclassOf<ALightBeamVFXManager>* BluePrint, float UltimateDamage, float DesapwenTime, FTimerManager* Manager, APlayerController* MainCharacterControllerPtr)
{
	HalfSize.Y = UltimateRadius;
	HalfSize.Z = UltimateRadius;
	CoolDownInSeconds = CoolDown;
	EnergyNeeded = Energy;
	LightBeamVFXManagerBlueprint = BluePrint;
	Damage = UltimateDamage;
	DespawnTimeInSeconds = DesapwenTime;
	TimerManager = Manager;
	MainCharacterController = MainCharacterControllerPtr;
}

void UUltimateAttack::StartAttack(FAttackParams FAttackParams)
{
	World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	ensure(World);
	Lantern = FAttackParams.Lantern;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	TSubclassOf<ALightBeamVFXManager> VFXToSpawn = *LightBeamVFXManagerBlueprint;
	FVector SpawnLocation = FAttackParams.Lantern->GetFireStartPos();
	FRotator SpawnRotation = FAttackParams.ShootDirection.Rotation();
	Orientation = SpawnRotation;
	ALightBeamVFXManager* NewVFX = World->SpawnActor<ALightBeamVFXManager>(VFXToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
	if (!NewVFX)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to spawn vfx for the ultimate attack"));
	}
	else 
	{
		CurrentVFX = NewVFX;
		StartPos = NewVFX->GetStartPosition();
		EndPos = NewVFX->GetEndPosition();
		float length = NewVFX->GetBeamLength();
		HalfSize.X = 5.f;
		NewVFX->BeamDelegate.AddDynamic(this, &UUltimateAttack::OnBeamFired);
	}

	ensure(MainCharacterController);
	MainCharacterController->SetIgnoreLookInput(true);
	MainCharacterController->SetIgnoreMoveInput(true);
}

void UUltimateAttack::OnBeamFired()
{
	TimerManager->SetTimer(DespawnTimerHandle, this, &UUltimateAttack::DestryBeamVFX, DespawnTimeInSeconds, false);
	if (AnimController) 
	{
		AnimController->SetAttackState((int)AttackState::UltimateFire);
	}
	TArray<AActor*> IgnoredActors;
	TArray<FHitResult> HitResults;
	FColor TraceColor = FColor::Red;
	FColor HitColor = FColor::Green;
	float DebugDrawTime = 5.0f;
	ensure(Lantern);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	bool bHit = UKismetSystemLibrary::BoxTraceMultiForObjects(Lantern, StartPos, EndPos, HalfSize, Orientation, ObjectTypes, false, IgnoredActors, EDrawDebugTrace::Type::ForDuration, HitResults, true, TraceColor, HitColor, DebugDrawTime);
	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* ActorHit = Hit.GetActor();
			if (!ActorHit) continue;

			UEnemyHealthComponent* HealthComponent = ActorHit->FindComponentByClass<UEnemyHealthComponent>();
			//ensure(ActorHit);
			if (HealthComponent)
			{
				//AEnemy* EnemyHit = Cast<AEnemy>(ActorHit);
				//ensure(EnemyHit);
				HealthComponent->GetHurt(Damage);
			}
			UE_LOG(LogTemp, Log, TEXT("Hit: %s at location %s"), *ActorHit->GetName(), *Hit.ImpactPoint.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No hits detected."));
	}
	ensure(World);
	DrawDebugBox(World, StartPos, HalfSize, FQuat(Orientation), FColor::Blue, false, DebugDrawTime);
	DrawDebugBox(World, EndPos, HalfSize, FQuat(Orientation), FColor::Blue, false, DebugDrawTime);

	ensure(MainCharacterController);
	MainCharacterController->SetIgnoreLookInput(false);
	MainCharacterController->SetIgnoreMoveInput(false);
}

void UUltimateAttack::DestryBeamVFX()
{
	if (CurrentVFX) 
	{
		CurrentVFX->Destroy();
	}
}
