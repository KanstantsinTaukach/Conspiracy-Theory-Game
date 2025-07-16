// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Player/CTGCharacter.h"

#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "Kismet/GameplayStatics.h"
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

ACTGCharacter::ACTGCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->SocketOffset = FVector(0.0f, 90.0f, 0.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArm);
    CameraComponent->bUsePawnControlRotation = false;

    InteractionComponent = CreateDefaultSubobject<UCTGInteractionComponent>("InteractionComponent");

    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
    GetCharacterMovement()->MaxWalkSpeedCrouched = 200.f;
}

void ACTGCharacter::BeginPlay()
{
    Super::BeginPlay();

    if(const auto PC = Cast<APlayerController>(Controller))
    {
        if(const auto InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            InputSystem->AddMappingContext(MappingContext, 0);
        }
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
            KsilanCharacter->SetOwnerActor(this);;
        }
    }
}

void ACTGCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACTGCharacter::SetIsChased(bool bChased)
{
    bIsChased = bChased;
}

void ACTGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if(const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ACTGCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACTGCharacter::Look);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ACTGCharacter::StartCrouch);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACTGCharacter::StopCrouch);
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACTGCharacter::PrimaryInteract);
        EnhancedInputComponent->BindAction(StunAction, ETriggerEvent::Triggered, this, &ACTGCharacter::TryStunEnemies);
    }
}

void ACTGCharacter::OnStunMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{

    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ACTGCharacter::Move(const FInputActionValue& Value) 
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

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

void ACTGCharacter::PrimaryInteract()
{
    if (bIsInteracting)
    {
        UE_LOG(LogTemp, Warning, TEXT("Already interacting, ignoring input"));
        return;
    }

    // Звук
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
                GetCharacterMovement()->DisableMovement();

                FOnMontageEnded EndDelegate;
                EndDelegate.BindUObject(this, &ACTGCharacter::OnInteractMontageEnded);
                AnimInstance->Montage_SetEndDelegate(EndDelegate, InteractMontage);
            }
        }
    }
}

void ACTGCharacter::TryStunEnemies()
{
    if (!bCanStun)
    {
        UE_LOG(LogTemp, Warning, TEXT("Stun on cooldown"));
        return;
    }


    if (StunMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {

            AnimInstance->Montage_Play(StunMontage.Get());


            GetCharacterMovement()->DisableMovement();


            FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(StunMontage.Get());
            if (MontageInstance)
            {
                MontageInstance->OnMontageEnded.BindUObject(this, &ACTGCharacter::OnStunMontageEnded);
            }
            bCanStun = false;
            GetWorldTimerManager().SetTimer(StunCooldownTimer, this, &ACTGCharacter::ResetStun, StunCooldown, false);
        }
    }


    FVector Start = CameraComponent->GetComponentLocation();
    FVector ForwardVector = CameraComponent->GetForwardVector();
    FVector End = Start + ForwardVector * StunDistance;


    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    float SphereRadius = 50.f;

    TArray<FHitResult> HitResults;

    bool bHit = GetWorld()->SweepMultiByChannel(
        HitResults, Start, End, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(SphereRadius), QueryParams);

    if (bHit)
    {
        for (const FHitResult& Hit : HitResults)
        {
            AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Hit.GetActor());
            if (!Enemy) continue;

            FVector ToTarget = (Enemy->GetActorLocation() - Start);
            float Distance = ToTarget.Size();
            ToTarget.Normalize();

            float DotProduct = FVector::DotProduct(ForwardVector, ToTarget);
            float Degrees = FMath::Acos(DotProduct) * (180.f / PI);

            if (Degrees <= StunConeAngle)
            {
                if (!Enemy->bIsStunned)
                {
                    Enemy->Stun();
                    UE_LOG(LogTemp, Log, TEXT("Stunned enemy %s, angle %.1f, distance %.1f"), *Enemy->GetName(), Degrees, Distance);
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

void ACTGCharacter::ResetStun()
{
    bCanStun = true;
    UE_LOG(LogTemp, Log, TEXT("Stun cooldown reset"));
}

void ACTGCharacter::OnInteractMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    bIsInteracting = false;
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}
FVector ACTGCharacter::GetPawnViewLocation() const
{
    return CameraComponent->GetComponentLocation();
}