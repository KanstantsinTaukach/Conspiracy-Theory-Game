// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Player/CTGCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CTGInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ACTGCharacter::ACTGCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->bUsePawnControlRotation = true;

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
}

void ACTGCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
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
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ACTGCharacter::PrimaryInteract);
    }
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
    if(InteractionComponent)
    {
        InteractionComponent->PrimaryInteract();
    }
}

FVector ACTGCharacter::GetPawnViewLocation() const
{
    return CameraComponent->GetComponentLocation();
}
