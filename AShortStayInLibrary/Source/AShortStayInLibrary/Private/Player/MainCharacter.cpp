// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainCharacter.h"
#include "InputActionValue.h"
#include "EnergySource/EnergySource.h"
#include "Book/BookEffectsManager.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bTickEvenWhenPaused = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(GetRootComponent());

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));  
    Camera->SetupAttachment(SpringArm);

    PlayerCameraController = CreateDefaultSubobject<UPlayerCameraController>(TEXT("CameraController"));
    PlayerCameraController->SetUpCamera(SpringArm, Camera);

    CharacterMovementComponent = GetCharacterMovement(); 
    ensure(CharacterMovementComponent);
    CharacterMovementComponent->bOrientRotationToMovement = true; 
    CharacterMovementComponent->AirControl = 0.2f; 

    PlayerAttackManager = CreateDefaultSubobject<UPlayerAttackManger>(TEXT("AttackManager"));

    LanternSocket = CreateDefaultSubobject<USceneComponent>(TEXT("Lantern Socket"));
    LanternSocket->SetupAttachment(RootComponent);

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Trigger Component"));
    SphereComponent->SetupAttachment(RootComponent);
    SphereComponent->SetSphereRadius(SphereComponentRadius);
    SphereComponent->SetCollisionProfileName(TEXT("Trigger"));
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnEnterTrigger);
    SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnTriggerEndOverlap);

    BookManager = CreateDefaultSubobject<UBookEffectsManager>(TEXT("Book Effects Manager"));
    BookManager->MainCharacter = this;
    BookManager->PlayerAttackManager = PlayerAttackManager;
    BookManager->WilloWispBP = &(PlayerAttackManager->WilloWispBluePrint);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
 
    SkeletalMesh = GetMesh();
    FRotator SpawnRotation = FRotator::ZeroRotator;
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    UWorld* World = GetWorld();
    if (World) 
    {
        Lantern = GetWorld()->SpawnActor<ALantern>(LanternBlueprint, GetActorLocation(), SpawnRotation, SpawnParams);
        if (Lantern) 
        {
            LanternSocket->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GunSocket"));
            Lantern->AttachToComponent(LanternSocket, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
            IsLanternAttached = true;
        } 
    }
    AController* CharacterController = GetController();
    if (Controller) 
    {
        MainCharacterController = Cast<APlayerController>(Controller);
    }
    if (MainCharacterController) 
    {
        if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MainCharacterController->GetLocalPlayer()))
        {
            InputSubsystem->AddMappingContext(CharacterContext, MainCharacterInputMappingConTextPriority);
            InputSubsystem->AddMappingContext(UIContext, UIInputMappingContextPriority);
        }
        CharacterMovementComponent->JumpZVelocity = JumpVelocity;
    }

    Health = NewObject<UHealth>(this, UHealth::StaticClass());
    Health->SetValue(MaxHealth);
    Energy = NewObject<UEnergy>(this, UEnergy::StaticClass());
    Energy->SetValue(MaxEnergy);

    if (HUDBP && MainCharacterController)
    {
        HUD = CreateWidget<UAShortStayInLibraryHUD>(MainCharacterController, HUDBP);
        ensure(HUD);
        HUD->AddToPlayerScreen();
        HUD->SetPlayerController(MainCharacterController);
        HUD->SetMainCharacter(this);
        Health->SetHUD(HUD);
        Energy->SetHUD(HUD);
        Health->SetMax(MaxHealth);
        Health->SetValue(MaxHealth);
        Energy->SetMax(MaxEnergy);
        Energy->SetValue(MaxEnergy);
        PlayerAttackManager->HUD = HUD;
        BookManager->HUD = HUD;
        HUD->SetShowCursor(false);
    }
}

void AMainCharacter::OnEnterTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        if (OtherActor->IsA(AEnergySource::StaticClass()))
        {
            AEnergySource* CurrentEnergySource = Cast<AEnergySource>(OtherActor);
            ensure(CurrentEnergySource);
            EnergySource = CurrentEnergySource;
            HUD->ChangeVisibilityEnergySourceInteractText(true);
        }
    }
}

void AMainCharacter::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor) 
    {
        if (OtherActor->IsA(AEnergySource::StaticClass())) 
        {
            EnergySource = nullptr;
            HUD->ChangeVisibilityEnergySourceInteractText(false);
        }
    }
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Yellow, FString::Printf(TEXT("Current Movement State: %d"), MovementState));
    if (MovementState == MovementState::Landing || MovementState == MovementState::Jumping)
    {
        if (CharacterMovementComponent->IsMovingOnGround()) 
        {
            MovementState = MovementState::Idle;
            if (AnimController)
            {
                AnimController->SetMovementState((int)MovementState::Idle);
            }
        }
    }
    if (!CharacterMovementComponent->IsMovingOnGround())
    {
        CheckJumpingState();
    }

    if (!PlayerAttackManager->MainCharacterController) 
    {
        PlayerAttackManager->MainCharacterController = MainCharacterController;
    }
    if (!PlayerAttackManager->AnimController) 
    {
        PlayerAttackManager->AnimController = AnimController;
    }

#if !UE_BUILD_SHIPPING
    FRotator ControllerRotation = Controller->GetControlRotation();
    FVector ForwardDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
    DrawDebugLine(GetWorld(), Lantern->GetFireStartPos(), ForwardDirection * 100.f + Lantern->GetFireStartPos(), FColor::Red, false, -1.f, 0U, 1.f);
    LanternSocket->SetWorldRotation(ForwardDirection.Rotation());
#endif
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedPlayerInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedPlayerInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
        EnhancedPlayerInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMainCharacter::StopMoving);
        EnhancedPlayerInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
        EnhancedPlayerInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::Jump);
        EnhancedPlayerInputComponent->BindAction(BeamAttackAction, ETriggerEvent::Started, this, &AMainCharacter::BeamAttack);
        EnhancedPlayerInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AMainCharacter::StartAiming);
        EnhancedPlayerInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AMainCharacter::EndAiming);
        EnhancedPlayerInputComponent->BindAction(WilloWispAction, ETriggerEvent::Started, this, &AMainCharacter::WilloWispAttack);
        EnhancedPlayerInputComponent->BindAction(UltimateAttackAction, ETriggerEvent::Started, this, &AMainCharacter::UltimateAttack);
        EnhancedPlayerInputComponent->BindAction(InteractEnergySourceAction, ETriggerEvent::Started, this, &AMainCharacter::InteractWithEnergySource);
        EnhancedPlayerInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AMainCharacter::PauseGame);
    }
}

MovementState AMainCharacter::GetCurrentMovementState()
{
    return MovementState;
}

UHealth* AMainCharacter::GetHealth()
{
    return Health;
}

void AMainCharacter::SetIsReading(bool IsReadingBook)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Reading: %d"), IsReadingBook));
    IsReading = IsReadingBook;
}

//void AMainCharacter::ResetPlayerStatus()
//{
//    // TODO: Reset player location to spawn location; Reset all stats 
//    Energy->SetMax(MaxEnergy);
//    Energy->SetValue(MaxEnergy);
//    Health->SetMax(MaxHealth);
//    Health->SetValue(MaxHealth);
//}

void AMainCharacter::Move(const FInputActionValue& Value)
{
    if (MovementState == MovementState::Jumping || MovementState == MovementState::Landing) return;
    MovementState = MovementState::Running;
    if (AnimController)
    {
        AnimController->SetMovementState((int)MovementState::Running);
    }
    const FInputActionValue::Axis2D InputDirection = Value.Get<FVector2D>();
    if (MainCharacterController)
    {
        AddMovementInput(GetActorForwardVector(), InputDirection.Y);
        AddMovementInput(GetActorRightVector(), InputDirection.X);
    }
}

void AMainCharacter::StopMoving(const FInputActionValue& Value) 
{
    MovementState = MovementState::Idle;
    if (AnimController) 
    {
        AnimController->SetMovementState((int)MovementState::Idle);
    }
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerPitchInput(LookAxisVector.Y);
    AddControllerYawInput(LookAxisVector.X);
}

void AMainCharacter::Jump(const FInputActionValue& Value) 
{
    bool ShouldJump = Value.Get<bool>();
    if (ShouldJump) 
    {
        ACharacter::Jump(); 
    }
    else 
    {
        ACharacter::StopJumping();
    }
}

void AMainCharacter::StartAiming(const FInputActionValue& Value)
{
    if (Value.Get<bool>()) 
    {
        GEngine->AddOnScreenDebugMessage(11, 3.f, FColor::Cyan, TEXT("Aiming starts"));
        PlayerAttackManager->AimStatusChange(true);
        PlayerCameraController->StartAiming();
        if (Lantern) 
        {
            Lantern->SetLaserVisibility(true);
        }
        if (AnimController) 
        {
            AnimController->SetAim(true);
        }
    }
}

void AMainCharacter::EndAiming(const FInputActionValue& Value)
{
    if (!Value.Get<bool>())
    {
        GEngine->AddOnScreenDebugMessage(11, 3.f, FColor::Cyan, TEXT("Aiming Ends"));
        PlayerAttackManager->AimStatusChange(false);
        PlayerCameraController->StopAiming();
        if (Lantern)
        {
            Lantern->SetLaserVisibility(false);
        }
        if (AnimController)
        {
            AnimController->SetAim(false);
        }
    }
}

void AMainCharacter::BeamAttack(const FInputActionValue& Value)
{
    FRotator ControllerRotation = Controller->GetControlRotation();
    FVector CamDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
    FAttackParams AttackParams;
    AttackParams.ShootDirection = CamDirection;
    AttackParams.Lantern = Lantern;
    PlayerAttackManager->Attack("Beam", Energy, AttackParams);
}

void AMainCharacter::WilloWispAttack(const FInputActionValue& Value)
{
    FRotator ControllerRotation = Controller->GetControlRotation();
    FVector ForwardDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
    FAttackParams AttackParams;
    AttackParams.ShootDirection = ForwardDirection;
    AttackParams.Lantern = Lantern;
    if (PlayerAttackManager->IfHaveWilloWisps()) 
    {
        GEngine->AddOnScreenDebugMessage(30, 3.f, FColor::Cyan, TEXT("Sending WilloWisps"));
        PlayerAttackManager->Attack("WilloWisp Sending", Energy, AttackParams);
    }
    else 
    {
        PlayerAttackManager->Attack("WilloWisp Spawning", Energy, AttackParams);
    }
}

void AMainCharacter::UltimateAttack(const FInputActionValue& Value) 
{
    FRotator ControllerRotation = Controller->GetControlRotation();
    FVector CamDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
    FAttackParams AttackParams;
    AttackParams.ShootDirection = CamDirection;
    AttackParams.Lantern = Lantern;
    PlayerAttackManager->Attack("Ultimate", Energy, AttackParams);
}

void AMainCharacter::InteractWithEnergySource(const FInputActionValue& Value)
{
    if (EnergySource) 
    {
        float EnergyReceived = EnergySource->LoseEnergy();
        Energy->ChangeValueByOffset(EnergyReceived);
    }
}

void AMainCharacter::PauseGame(const FInputActionValue& Value)
{
    if (IsReading) return;

    if (HUD) 
    {
        if (HUD->GetPauseMenuVisible()) 
        {
            HUD->SetPauseMenuVisibility(false);
        }
        else 
        {
            HUD->SetPauseMenuVisibility(true);
        }
    }
}

void AMainCharacter::CheckJumpingState()
{
    if (GetVelocity().Z < 0)
    {
        MovementState = MovementState::Landing;
        if (AnimController)
        {
            AnimController->SetMovementState((int)MovementState::Landing);
        }
    }
    else 
    {
        MovementState = MovementState::Jumping;
        if (AnimController)
        {
            AnimController->SetMovementState((int)MovementState::Jumping);
        }
    }
}
