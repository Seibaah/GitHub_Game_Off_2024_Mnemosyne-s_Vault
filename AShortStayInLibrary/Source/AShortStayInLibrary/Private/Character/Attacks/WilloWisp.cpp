// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attacks/WilloWisp.h"
#include "Enemy/EnemyHealthComponent.h"

// Sets default values
AWilloWisp::AWilloWisp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WilloWispMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	SetRootComponent(WilloWispMesh);

	InnerTriggerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Inner Trigger Component"));
	InnerTriggerComponent->SetupAttachment(RootComponent);
	InnerTriggerComponent->SetSphereRadius(InnerTriggerRadiusInCM);
	InnerTriggerComponent->SetCollisionProfileName(TEXT("Inner WilloWisp Trigger"));
	InnerTriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &AWilloWisp::OnTriggerEnterInner);
	

	OuterTriggerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Outer Trigger Component"));
	OuterTriggerComponent->SetupAttachment(RootComponent);
	OuterTriggerComponent->SetSphereRadius(OuterTriggerRadiusInCM);
	OuterTriggerComponent->SetCollisionProfileName(TEXT("Outer WilloWisp Trigger"));
	OuterTriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &AWilloWisp::OnTriggerEnterOuter);

	SpotLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Spot Light"));
	SpotLight->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWilloWisp::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		TimerManager = &World->GetTimerManager();
	}
}

void AWilloWisp::Fire(FVector AimDirection, const FVector& LanternPos)
{
	AimingDirecion = AimDirection;
	State = WilloWispState::Active;
	FVector Destination = AimDirection * AimingDirectionOffset + LanternPos;
	MovementDirection = (Destination - GetActorLocation()).GetSafeNormal();
	FullEnergyLevel = EnergyLevel;
	TimerManager->SetTimer(TimerHandle, 1 / EnergyDissipateRate, false);
	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AWilloWisp::Move(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = CurrentLocation + Speed * DeltaTime * MovementDirection;
	SetActorLocation(NewLocation);
}

void AWilloWisp::CalculateSpeedDamageDirectionLight(float DeltaTime)
{
	if (State == WilloWispState::Active) 
	{
		MovementDirection = FMath::Lerp(MovementDirection, AimingDirecion, LerpSpeed * DeltaTime).GetSafeNormal();
		SpotLight->SetIntensity(EnergyLevel * LightIntensityMultiplier);
	}
	else if (State == WilloWispState::ChasingEnemy && EnemyBeingChased) 
	{
		FVector EnemyPos = EnemyBeingChased->GetActorLocation();
		MovementDirection = (EnemyPos - GetActorLocation()).GetSafeNormal();
	}
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + MovementDirection * 500.f, FColor::Yellow, false, -1, 0, 5.f);
	float TotalTime = 1 / EnergyDissipateRate;
	float TimeElapsedPercentage = TimerManager->GetTimerElapsed(TimerHandle) / TotalTime;
	EnergyLevel = FullEnergyLevel * (1 - TimeElapsedPercentage);
	GEngine->AddOnScreenDebugMessage(52, -1.f, FColor::Red, FString::Printf(TEXT("Energy: %f"), EnergyLevel));	
	Speed = WilloWispSpeedMultiplier * EnergyLevel;
	Damage = DamageMultiplier * EnergyLevel;
}

void AWilloWisp::Die()
{
	Destroy();
}

void AWilloWisp::OnTriggerEnterInner(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UEnemyHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UEnemyHealthComponent>();
		if (State != WilloWispState::Idle && HealthComponent)
		{
			HealthComponent->GetHurt(Damage);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Inner Trigger Hit"));
			Die();
		}
		/*if (State != WilloWispState::Idle && OtherActor->IsA(AEnemy::StaticClass()))
		{
			AEnemy* Enemy = Cast<AEnemy>(OtherActor);
			ensure(Enemy);
			Enemy->GetHurt(Damage);
			Die();
		}*/
	}
}

void AWilloWisp::OnTriggerEnterOuter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UEnemyHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UEnemyHealthComponent>();
		if (State == WilloWispState::Active && HealthComponent)
		{
			GEngine->AddOnScreenDebugMessage(60, 5.f, FColor::Cyan, TEXT("HitEnemy"));
			EnemyBeingChased = Cast<AActor>(OtherActor);
			ensure(EnemyBeingChased);
			State = WilloWispState::ChasingEnemy;
		}
	}
}

// Called every frame
void AWilloWisp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (State != WilloWispState::Idle)
	{
		CalculateSpeedDamageDirectionLight(DeltaTime);
		Move(DeltaTime);
	}

	if (State == WilloWispState::Active && !TimerManager->IsTimerActive(TimerHandle))
	{
		Die();
	}
}