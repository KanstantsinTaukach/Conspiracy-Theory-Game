// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Player/CTGCharacter.h"
#include "Engine/World.h"
#include "Player/CTGPlayerState.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AIPerceptionSystem.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/PointLightComponent.h"
#include "TimerManager.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "PhysicsEngine/BodyInstance.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Enemy/EnemyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CTGInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/CTGKsilanCharacter.h"
#include "Components/CTGCharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CTGStaminaComponent.h"

void ACTGCharacter::SetBossRoomLocation(const FVector& Location)
{
    BossRoomLocation = Location;
    bShowCompassArrow = true;
    if (CompassArrowMesh)
    {
        CompassArrowMesh->SetVisibility(true);  
    }
    UE_LOG(LogTemp, Warning, TEXT("BossRoomLocation set to: %s"), *BossRoomLocation.ToString());
}

ACTGCharacter::ACTGCharacter(const FObjectInitializer& OfjInit)
    : Super(OfjInit.SetDefaultSubobjectClass<UCTGCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;
    bIsChased = false ;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->SocketOffset = FVector(0.0f, 90.0f, 0.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArm);
    CameraComponent->bUsePawnControlRotation = false;

    InteractionComponent = CreateDefaultSubobject<UCTGInteractionComponent>("InteractionComponent");

    StaminaComponent = CreateDefaultSubobject<UCTGStaminaComponent>("StaminaComponent");

    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
    GetCharacterMovement()->MaxWalkSpeedCrouched = 200.f;

    CompassArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("CompassArrow"));
    CompassArrow->SetupAttachment(RootComponent);
    CompassArrow->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
    CompassArrow->SetArrowColor(FColor::Red);
    CompassArrow->SetHiddenInGame(true);

    CompassArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CompassArrowMesh"));
    CompassArrowMesh->SetupAttachment(CompassArrow);
    CompassArrowMesh->SetRelativeLocation(FVector::ZeroVector);
}

void ACTGCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (!Tags.Contains("Player"))
    {
        Tags.Add("Player");
    }
    ScheduleNextXylanShout();
    if (const auto PC = Cast<APlayerController>(Controller))
    {
        if (const auto InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            InputSystem->AddMappingContext(MappingContext, 0);
        }
    }
    if (APlayerController* PC = Cast<APlayerController>(Controller))
    {
        CachedPlayerState = PC->GetPlayerState<ACTGPlayerState>();
        if (CachedPlayerState)
        {
            CachedPlayerState->OnPointsChanged.AddDynamic(this, &ACTGCharacter::OnPointsChanged);
            CompassScoreThreshold = CachedPlayerState->GetPointsToUnlockBoss();
        }
    }
    if (CompassArrow)
    {
        CompassArrow->SetHiddenInGame(true); 
    }

    if (CompassArrowMesh)
    {
        CompassArrowMesh->SetVisibility(false);  
    }
    if (GetWorld() && KsilanClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * (-50.0f) + FVector(0.0f, 0.0f, 25.0f);
        FRotator SpawnRotation = GetActorRotation();

        auto* KsilanCharacter = GetWorld()->SpawnActor<ACTGKsilanCharacter>(KsilanClass, SpawnLocation, SpawnRotation, SpawnParams);

        if (KsilanCharacter)
        {
            KsilanCharacter->SetOwnerActor(this);
        }
    }

    check(StaminaComponent);
    StaminaComponent->OnStaminaEmpty.AddUObject(this, &ACTGCharacter::OnStaminaEmpty);
}

void ACTGCharacter::PerformXylanShout()
{
    if (XylanShoutSounds.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No XylanShoutSounds set!"));
        return;
    }


    int32 Index = FMath::RandRange(0, XylanShoutSounds.Num() - 1);
    USoundBase* SelectedShout = XylanShoutSounds[Index];


    UGameplayStatics::PlaySoundAtLocation(this, SelectedShout, GetActorLocation());


    UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), ShoutLoudness, this, ShoutAggroRadius, FName("XylanShout"));

    UE_LOG(LogTemp, Log, TEXT("Xylan shout played: %s"), *SelectedShout->GetName());
    if (XylanShoutWidgetClass && !XylanShoutWidgetInstance)
    {
        if (APlayerController* PC = Cast<APlayerController>(GetController()))
        {
            XylanShoutWidgetInstance = CreateWidget<UUserWidget>(PC, XylanShoutWidgetClass);
            if (XylanShoutWidgetInstance)
            {
                XylanShoutWidgetInstance->AddToViewport();
            }
        }
    }

    ScheduleNextXylanShout();
}

void ACTGCharacter::ScheduleNextXylanShout()
{
    float NextInterval = FMath::FRandRange(MinShoutInterval, MaxShoutInterval);
    GetWorldTimerManager().SetTimer(XylanShoutTimerHandle, this, &ACTGCharacter::PerformXylanShout, NextInterval, false);
}

void ACTGCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Score >= CompassScoreThreshold && !BossRoomLocation.IsZero())
    {

        UpdateCompass();
    }
    else
    {
        if (!CompassArrow->bHiddenInGame) CompassArrow->SetHiddenInGame(true);
    }
}



void ACTGCharacter::OnPointsChanged(ACTGPlayerState* PS, int32 NewPoints, int32 Delta)
{
    Score = NewPoints;

    UE_LOG(LogTemp, Warning, TEXT("Points changed: %d"), NewPoints);

    if (NewPoints >= CompassScoreThreshold)
    {
        bShowCompassArrow = true;



        if (CompassArrowMesh)
        {
            CompassArrowMesh->SetVisibility(true);  
        }
    }
    else
    {
        bShowCompassArrow = false;

        if (CompassArrow)
        {
            CompassArrow->SetHiddenInGame(true);
        }

        if (CompassArrowMesh)
        {
            CompassArrowMesh->SetVisibility(false);  
        }
    }
}

void ACTGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ACTGCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACTGCharacter::Look);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ACTGCharacter::StartCrouch);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACTGCharacter::StopCrouch);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ACTGCharacter::OnStartSprinting);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACTGCharacter::OnStopSprinting);
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACTGCharacter::PrimaryInteract);
        EnhancedInputComponent->BindAction(StunAction, ETriggerEvent::Triggered, this, &ACTGCharacter::TryStunEnemies);
    }
}

void ACTGCharacter::OnStunMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{

    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        PC->EnableInput(PC);
    }
}

void ACTGCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    IsMoving = (MovementVector.X != 0.0f || MovementVector.Y != 0.0f);

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(ForwardDirection, MovementVector.Y);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(RightDirection, MovementVector.X);
}

void ACTGCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();

    AddControllerPitchInput(LookAxisVector.Y);
    AddControllerYawInput(LookAxisVector.X);
}

void ACTGCharacter::StartCrouch(const FInputActionValue& Value)
{
    Crouch();
}

void ACTGCharacter::StopCrouch(const FInputActionValue& Value)
{
    UnCrouch();
}

void ACTGCharacter::OnStartSprinting()
{
    if (StaminaComponent->CanSprinting())
    {
        WantsToSprint = true;
        if (!GetVelocity().IsZero() && IsMoving)
        {
            StaminaComponent->ChangeSpamina(WantsToSprint);
        }
        else
        {
            OnStopSprinting();
        }
    }
}

void ACTGCharacter::OnStopSprinting()
{
    WantsToSprint = false;
    StaminaComponent->ChangeSpamina(WantsToSprint);
}

bool ACTGCharacter::IsCharacterRunning() const
{
    return WantsToSprint && IsMoving && !GetVelocity().IsZero();
}

void ACTGCharacter::OnStaminaEmpty()
{
    OnStopSprinting();
}

void ACTGCharacter::PrimaryInteract()
{
    if (bIsInteracting)
    {
        UE_LOG(LogTemp, Warning, TEXT("Already interacting, ignoring input"));
        return;
    }
    SpawnStunFlash();
    TryStunEnemies();
    if (InteractSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, InteractSound, GetActorLocation());
    }

    if (InteractionComponent)
    {
        InteractionComponent->PrimaryInteract();
    }

    if (InteractMontage && GetMesh())
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            float MontageDuration = AnimInstance->Montage_Play(InteractMontage);
            if (MontageDuration > 0.f)
            {
                bIsInteracting = true;
                if (APlayerController* PC = Cast<APlayerController>(GetController()))
                {
                    PC->DisableInput(PC);
                    GetCharacterMovement()->StopMovementImmediately();
                }

                FOnMontageEnded EndDelegate;
                EndDelegate.BindUObject(this, &ACTGCharacter::OnInteractMontageEnded);
                AnimInstance->Montage_SetEndDelegate(EndDelegate, InteractMontage);
            }
        }
    }
}

void ACTGCharacter::TryStunEnemies()
{
    if (!bCanStun) return;


    FRotator NewRotation = CameraComponent->GetComponentRotation();
    NewRotation.Pitch = 0.f;
    NewRotation.Roll = 0.f;
    SetActorRotation(NewRotation);


    if (StunMontage || !InteractMontage)
    {
        if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
        {
            AnimInstance->Montage_Play(StunMontage.Get());
            if (APlayerController* PC = Cast<APlayerController>(GetController()))
            {
                PC->DisableInput(PC);
                GetCharacterMovement()->StopMovementImmediately();
            }
            FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(StunMontage.Get());
            if (MontageInstance)
            {
                MontageInstance->OnMontageEnded.BindUObject(this, &ACTGCharacter::OnStunMontageEnded);
            }
            bCanStun = false;
            GetWorldTimerManager().SetTimer(StunCooldownTimer, this, &ACTGCharacter::ResetStun, StunCooldown, false);
        }
    }

    SpawnStunFlash();


    FVector Start = GetActorLocation() + FVector(0, 0, 50.f);
    FVector ForwardVector = GetActorForwardVector();
    FVector End = Start + ForwardVector * StunDistance;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    float SphereRadius = 100.f;
    TArray<FHitResult> HitResults;

    if (GetWorld()->SweepMultiByChannel(
            HitResults, Start, End, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(SphereRadius), QueryParams))
    {
        float CosConeAngle = FMath::Cos(FMath::DegreesToRadians(StunConeAngle));
        for (const FHitResult& Hit : HitResults)
        {
            AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Hit.GetActor());
            if (!Enemy) continue;

            FVector ToTarget = Enemy->GetActorLocation() - Start;
            ToTarget.Z = 0;
            ToTarget.Normalize();

            float DotProduct = FVector::DotProduct(ForwardVector, ToTarget);
            if (DotProduct >= CosConeAngle)
            {
                if (!Enemy->bIsStunned)
                {
                    Enemy->Stun();
                    UE_LOG(LogTemp, Log, TEXT("Stunned enemy %s"), *Enemy->GetName());
                }
            }
        }
    }

    DrawDebugSphere(GetWorld(), End, SphereRadius, 16, FColor::Green, false, 2.0f);
    FVector RightCone = ForwardVector.RotateAngleAxis(StunConeAngle, FVector::UpVector) * StunDistance;
    FVector LeftCone = ForwardVector.RotateAngleAxis(-StunConeAngle, FVector::UpVector) * StunDistance;
    DrawDebugLine(GetWorld(), Start, Start + RightCone, FColor::Yellow, false, 2.0f, 0, 1.5f);
    DrawDebugLine(GetWorld(), Start, Start + LeftCone, FColor::Yellow, false, 2.0f, 0, 1.5f);
}

void ACTGCharacter::SpawnStunFlash()
{
    FVector FlashLocation = GetActorLocation() + GetActorForwardVector() * 100.f + FVector(0, 0, 50.f);

    UPointLightComponent* StunFlash = NewObject<UPointLightComponent>(this);
    if (StunFlash)
    {
        StunFlash->RegisterComponent();
        StunFlash->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
        StunFlash->SetWorldLocation(FlashLocation);
        StunFlash->SetIntensity(10000.f);
        StunFlash->SetAttenuationRadius(400.f);
        StunFlash->SetLightColor(FLinearColor::White);
        StunFlash->SetVisibility(true);

        FTimerDelegate TimerDelegate;
        TimerDelegate.BindUObject(this, &ACTGCharacter::DestroyStunFlash, StunFlash);

        FTimerHandle FlashTimer;
        GetWorld()->GetTimerManager().SetTimer(FlashTimer, TimerDelegate, 0.1f, false);
    }
}

void ACTGCharacter::DestroyStunFlash(UPointLightComponent* Flash)
{
    if (Flash)
    {
        Flash->DestroyComponent();
    }
}

void ACTGCharacter::ResetStun()
{
    bCanStun = true;
    UE_LOG(LogTemp, Log, TEXT("Stun cooldown reset"));
}

void ACTGCharacter::OnInteractMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    bIsInteracting = false;
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        PC->EnableInput(PC);
        GetCharacterMovement()->StopMovementImmediately();
    }
}
FVector ACTGCharacter::GetPawnViewLocation() const
{
    return CameraComponent->GetComponentLocation();
}
void ACTGCharacter::UpdateCompass()
{
    FVector Direction = BossRoomLocation - GetActorLocation();
    Direction.Z = 0;
    if (Direction.IsNearlyZero()) return;

    Direction.Normalize();

    FRotator TargetRotation = Direction.Rotation();


    FRotator CurrentRotation = CompassArrow->GetComponentRotation();
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f);

    CompassArrow->SetWorldRotation(NewRotation);
}
void ACTGCharacter::SetIsChased(bool bChased)
{
    if (bIsChased != bChased)
    {
        bIsChased = bChased;
        UE_LOG(LogTemp, Warning, TEXT("Player chase state changed to: %s"), bIsChased ? TEXT("Chased") : TEXT("Not Chased"));


        if (bIsChased)
        {

        }
        else
        {

        }
    }
}
void ACTGCharacter::PlayFootstep()
{
    UE_LOG(LogTemp, Warning, TEXT("Player played footstep, crouched: %d, velocity: %f"), bIsCrouched, GetVelocity().Size());

    if (!GetCharacterMovement()->IsMovingOnGround()) return;

    USoundBase* SelectedSound = nullptr;
    float Loudness = 0.0f;
    float MaxRange = 0.0f;

    if (bIsCrouched && CrouchFootstepSounds.Num() > 0)
    {
        SelectedSound = CrouchFootstepSounds[FMath::RandRange(0, CrouchFootstepSounds.Num() - 1)];
        Loudness = CrouchFootstepLoudness;
        MaxRange = CrouchFootstepRange;
    }
    else if (GetVelocity().Size() < 300.f && WalkFootstepSounds.Num() > 0)
    {
        SelectedSound = WalkFootstepSounds[FMath::RandRange(0, WalkFootstepSounds.Num() - 1)];
        Loudness = WalkFootstepLoudness;
        MaxRange = WalkFootstepRange;
    }
    else if (RunFootstepSounds.Num() > 0)
    {
        SelectedSound = RunFootstepSounds[FMath::RandRange(0, RunFootstepSounds.Num() - 1)];
        Loudness = RunFootstepLoudness;
        MaxRange = RunFootstepRange;
    }

    if (SelectedSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, SelectedSound, GetActorLocation());

        UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), Loudness, this, MaxRange, FName("Footstep"));
    }
}
