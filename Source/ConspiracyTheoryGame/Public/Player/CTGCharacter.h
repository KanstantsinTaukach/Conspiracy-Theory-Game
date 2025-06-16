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
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* MappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* MovementAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* CrouchAction;

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crouch")
    //FVector CrouchEyeOffset;
    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crouch")
    //float CrouchSpeed;

    virtual void BeginPlay() override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    void StartCrouch(const FInputActionValue& Value);
    void StopCrouch(const FInputActionValue& Value);

//private:
//    void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
//    void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
//    void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;
};
