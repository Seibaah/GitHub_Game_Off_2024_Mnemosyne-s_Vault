// Fill out your copyright notice in the Description page of Project Settings.


#include "Book/FloatingBook.h"
#include "Camera/CameraActor.h"
#include "Book/BookEffectsManager.h"
#include "UI/AShortStayInLibraryHUD.h"

// Sets default values
AFloatingBook::AFloatingBook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

// Called when the game starts or when spawned
void AFloatingBook::BeginPlay()
{
	Super::BeginPlay();
	//Store the initial location of the book
	StartLocation = BookCollider ->GetComponentLocation();  
	if (BookCollider)
	{
		BookCollider->SetSimulatePhysics(true); // Start without physics
		BookCollider->SetEnableGravity(false); // Disable gravity
	}
}

void AFloatingBook::SetBookCollider(UShapeComponent* collider)
{
	BookCollider = collider;
}

void AFloatingBook::SetTargetActor(USceneComponent* Target)
{
	TargetActor = Target;
} 

void AFloatingBook::SetPageBookAnimInstances(TArray<UAnimInstance*> Instances)
{
	PageBookAnimInstances.Empty();  // Clear existing instances if needed
	PageBookAnimInstances = Instances;
	// Since the book opens from right to  left, the indeces are reversed
	BookDoubleIndex = FVector2D(Instances.Num(), Instances.Num()-1);
}

void AFloatingBook::SwitchActivePages(bool DisableLeftPage, bool OnOpenBook) 
{ 
	if (OnOpenBook) {
		for (int i = 0; i < PageBookAnimInstances.Num() - 1; i++)
		{
			PageBookAnimInstances[i]->GetSkelMeshComponent()->SetVisibility(false);
		}  
	}
	else {
		if (DisableLeftPage && XBufferIndex < PageBookAnimInstances.Num())
		{
			PageBookAnimInstances[XBufferIndex]->GetSkelMeshComponent()->SetVisibility(false);
		}          
		else if (!DisableLeftPage && YBufferIndex >= 0)
		{
			//print the actual buffer index 
			UE_LOG(LogTemp, Warning, TEXT("Buffer index: %d"), YBufferIndex);
			PageBookAnimInstances[YBufferIndex]->GetSkelMeshComponent()->SetVisibility(false);
		}
	}	
}

void AFloatingBook::SetReadBook()
{
	IsRead = true;
}

UTexture2D* AFloatingBook::GetTexture()
{
	return Texture;
}

void AFloatingBook::SetUITextVisibility(bool ShouldShow)
{
	if (HUD) 
	{
		HUD->ChangeVisibilityBookInteractText(ShouldShow);
	}
}

void AFloatingBook::PauseTime(bool ShouldPauseTime)
{
	if (HUD) 
	{
		HUD->ChangePauseGameStatus(ShouldPauseTime);
	}
}

//We want to get the left page when turning the page to the right
UAnimInstance* :: AFloatingBook::GetLeftPage()
{
	UAnimInstance* LeftPage = nullptr;
	// When getting the left page, the x index must be < Instances.Num()
	if (BookDoubleIndex.X < PageBookAnimInstances.Num())
	{
		LeftPage = PageBookAnimInstances[BookDoubleIndex.X];
		YBufferIndex = BookDoubleIndex.Y;
		BookDoubleIndex.Y = BookDoubleIndex.X; 
		BookDoubleIndex.X++;
		if (BookDoubleIndex.X < PageBookAnimInstances.Num())  
		{
			PageBookAnimInstances[BookDoubleIndex.X]->GetSkelMeshComponent()->SetVisibility(true);
		}		
	}
	if (LeftPage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Left page is null"));
	}
	return LeftPage;
}

//We want to get the right page when turning the page to the left
UAnimInstance* ::AFloatingBook::GetRightPage()
{
	UAnimInstance* RightPage = nullptr;
	// When getting the right page, the y index must be >= 0
	if (BookDoubleIndex.Y >= 0)
	{
		RightPage = PageBookAnimInstances[BookDoubleIndex.Y];
		XBufferIndex = BookDoubleIndex.X;
		BookDoubleIndex.X = BookDoubleIndex.Y;
		BookDoubleIndex.Y--;
		if (BookDoubleIndex.Y >= 0)
		{
			PageBookAnimInstances[BookDoubleIndex.Y]->GetSkelMeshComponent()->SetVisibility(true);
		}
	}
	if (RightPage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Right page is null"));
	}
	return RightPage;
}

void AFloatingBook::SetupAndApplyEffect()
{
	if (IsRead) return;
	ensure(BookEffectsManager);
	if (!BookEffectsManager) return;
	UBookEffectsManager::BookEffectFunction function = BookEffectsManager->BookEffects[BookID];
	(BookEffectsManager->*function)();
	Texture = BookEffectsManager->Textures[BookID];
	IsRead = true;
}

void AFloatingBook::SetBookID(int ID)
{
	BookID = ID;
}

int AFloatingBook::GetBookID()
{
	return BookID;
}

void AFloatingBook::SetHUD(UAShortStayInLibraryHUD* GameHUD)
{
	HUD = GameHUD;
}

void AFloatingBook::SetBookEffectsManager(UBookEffectsManager* Manager)
{
	BookEffectsManager = Manager;
}

// Called every frames
void AFloatingBook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	if (BookCollider) {
		DeltaZ = Amplitude * FMath::Sin(Frequency * RunningTime);
		NewLocation = BookCollider -> GetComponentLocation();
		NewLocation.Z += DeltaZ;
		BookCollider->SetWorldLocation(NewLocation);
		if (TargetActor)
		{
			HandleTargetInteraction();
			if (bLookAtTarget)
			{
				RotateTowardsTarget(TargetActor->GetComponentLocation()); 
			}
		}
	}
	RunningTime += DeltaTime;	
}
   


void AFloatingBook::RotateTowardsTarget(FVector Target)
{
	// Calculate the normalized direction to the target
	FVector TargetDirection = (Target - BookCollider -> GetComponentLocation()).GetSafeNormal();

	// Determine the rotation needed to face the target
	FRotator TargetRotation = FRotationMatrix::MakeFromX(TargetDirection).Rotator();

	// Smoothly interpolate the current rotation towards the target rotation
	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);

	// Apply the interpolated rotation to the actor 
	SetActorRotation(NewRotation);
}


void AFloatingBook::RestoreToHoverPosition()
{
	// Calculate the direction to the start location
	FVector Direction = (StartLocation - BookCollider -> GetComponentLocation()).GetSafeNormal();
	float DistanceToStart = FVector::Dist(StartLocation, BookCollider -> GetComponentLocation());

	// Calculate the force needed to move the book back to the start location
	FVector Force = Direction * ForceFieldStrength * 2;

	// Ensure the mesh component is valid and has physics enabled
	if (BookCollider && BookCollider ->IsSimulatingPhysics())
	{
		float t = DistanceToStart / RepelRadius;
		// Adjust the velocity of the book accordingly
		BookCollider ->SetPhysicsLinearVelocity(Force * t, false); 
	}
}

void AFloatingBook::HandleTargetInteraction()
{
	float Distance = FVector::Dist(TargetActor->GetComponentLocation(), BookCollider -> GetComponentLocation());
	switch (BookBehavior)
	{
	case EBookBehavior::Repellent:
		HandleRepellentBehavior(Distance);
		break;

	case EBookBehavior::Follower:
		HandleFollowerBehavior(Distance);
		break;

	case EBookBehavior::Interactable:
		if(bGoTowardsObjective) HandleInteractableBehavior(Distance);
		break;
	}
}

// Define the specific behaviors with FORCEINLINE to encourage compiler inlining

FORCEINLINE void AFloatingBook::HandleRepellentBehavior(float Distance)
{
	if (Distance < RepelRadius) {
		FVector ForceDirection = (BookCollider ->GetComponentLocation() - TargetActor->GetComponentLocation()).GetSafeNormal();
		float T = (RepelRadius - Distance) / RepelRadius;
		FVector Force = ForceDirection * ForceFieldStrength * T;
		BookCollider ->SetPhysicsLinearVelocity(Force, false);
	} 
	else {
		RestoreToHoverPosition();
	}
}

FORCEINLINE void AFloatingBook::HandleFollowerBehavior(float Distance)
{
	if (Distance > AttractRadius) {
		FVector ForceDirection = (TargetActor->GetComponentLocation() - BookCollider->GetComponentLocation()).GetSafeNormal();
		float T = ((Distance + 1 / DampeningFactor) - AttractRadius) / AttractRadius;
		FVector Force = ForceDirection * ForceFieldStrength * T;
		BookCollider->SetPhysicsLinearVelocity(Force, false);
	} 
	else {
		BookCollider->SetPhysicsLinearVelocity(FVector(0, 0, 0), false);
	}
}

FORCEINLINE void AFloatingBook::HandleInteractableBehavior(float Distance)
{
	if (Distance > InteractRadius && !bLockBookInPlace) {
		//UE_LOG(LogTemp, Warning, TEXT("Interacting with book"));
		FVector ForceDirection = (TargetActor->GetComponentLocation() - BookCollider -> GetComponentLocation()).GetSafeNormal();
		float T = ((Distance + 1/DampeningFactor) - InteractRadius) / InteractRadius;
		FVector Force = ForceDirection * ForceFieldStrength * T;
		BookCollider ->SetPhysicsLinearVelocity(Force, false);
	}
	else {
		bLockBookInPlace = true; 
		BookCollider ->SetPhysicsLinearVelocity(FVector(0, 0, 0), false);
	}
}


