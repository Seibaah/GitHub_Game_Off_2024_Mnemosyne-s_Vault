// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attacks/BeamAttack.h"
#include "Enemy/EnemyInterface.h"
#include "Enemy/EnemyHealthComponent.h"

UBeamAttack::UBeamAttack()
{
}

UBeamAttack::UBeamAttack(FString name, int Damage, float CoolDownInSeconds, int EnergyNeeded, float BeamDistanceInCM) : UAttack(name = "Beam Attack", Damage = 2, CoolDownInSeconds = 3.f, EnergyNeeded = 10), BeamDistanceInCM(BeamDistanceInCM = 1000.f)
{
}	

void UBeamAttack::SetUp(FString BeamAttackName, int BeamAttackDamage, float BeamAttackCoolDownInSeconds, int BeamAttackEnergyNeeded, float BeamAttackDistanceInCM, TSubclassOf<AActor>* beamVFXBP)
{
	Name = BeamAttackName;
	Damage = BeamAttackDamage;
	CoolDownInSeconds = BeamAttackCoolDownInSeconds;
	EnergyNeeded = BeamAttackEnergyNeeded;
	BeamDistanceInCM = BeamAttackDistanceInCM;
	BeamVFXBlueprint = beamVFXBP;
}

void UBeamAttack::StartAttack(FAttackParams AttackParms)
{
	GEngine->AddOnScreenDebugMessage(4, 1.f, FColor::Green, FString::Printf(TEXT("BeamAttack startAttack called %f"), BeamDistanceInCM));
	FVector Start = AttackParms.Lantern->GetFireStartPos();
	FVector ForwardVector = AttackParms.ShootDirection;
	
	FVector End = Start + (ForwardVector * BeamDistanceInCM);

	if (BeamVFXBlueprint) 
	{
		UWorld* World = GetWorld();
		ensure(World);
		FVector SpawnLocation = AttackParms.Lantern->GetFireStartPos();
		FRotator SpawnRotation = AttackParms.ShootDirection.Rotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		World->SpawnActor<AActor>(*BeamVFXBlueprint, SpawnLocation, SpawnRotation, SpawnParams);
	}
	TArray<FHitResult> HitResults;
	FCollisionQueryParams TraceParams(FName(TEXT("BeamAttackRayCast")), true);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;

	bool bHit = GetWorld()->LineTraceMultiByObjectType(HitResults, Start, End, ECC_Pawn, TraceParams);

	if (bHit)
	{
		GEngine->AddOnScreenDebugMessage(4, 5.f, FColor::Red, FString::Printf(TEXT("Hit %d actor(s)"), HitResults.Num()));
		for (FHitResult Result : HitResults)
		{
			AActor* HitActor = Result.GetActor();
			if (HitActor) 
			{
				UEnemyHealthComponent* HealthComponent = HitActor->FindComponentByClass<UEnemyHealthComponent>();

				if (HealthComponent) 
				{
					HealthComponent->GetHurt(Damage);
				}
			}
			//if (HitActor && HitActor->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
			//{
			//	GEngine->AddOnScreenDebugMessage(6, 5.f, FColor::Red, FString::Printf(TEXT("Detected hit on: %s"), *HitActor->GetName()));
			//	//IEnemyInterface* HitEnemy = Cast<IEnemyInterface>(HitActor);
			//	//HitEnemy->GetHurt(Damage);
			//}
		}
	}

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.5f, 0, 5.f);
}
