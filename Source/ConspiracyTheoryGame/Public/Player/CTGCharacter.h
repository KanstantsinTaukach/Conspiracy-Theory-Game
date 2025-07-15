// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CTGCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UCTGInteractionComponent;
class ACTGKsilanCharacter;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ACTGCharacter();

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USpringArmComponent> SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UCameraComponent> CameraComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UCTGInteractionComponent> InteractionComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> MappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> MovementAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> CrouchAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> InteractAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> StunAction;

    UPROPERTY(EditDefaultsOnly, Category = "Stun")
    float StunDistance = 1000.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Stun")
    float StunRadius = 300.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Spawning")
    TSubclassOf<ACTGKsilanCharacter> KsilanClass;

    virtual void BeginPlay() override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    void StartCrouch(const FInputActionValue& Value);
    void StopCrouch(const FInputActionValue& Value);

    void PrimaryInteract();
    void TryStunEnemies();

    virtual FVector GetPawnViewLocation() const override;
};
