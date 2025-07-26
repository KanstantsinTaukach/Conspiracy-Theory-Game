// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NiagaraSystem.h"
#include "Components/ArrowComponent.h"
#include "CTGCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UCTGInteractionComponent;
class ACTGKsilanCharacter;
class UCTGStaminaComponent;
class UPointLightComponent;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    UPROPERTY()
    UUserWidget* XylanShoutWidgetInstance;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> XylanShoutWidgetClass;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stun")
    bool bCanStun = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stun")
    bool bIsStunned = false;

    UPROPERTY(BlueprintReadOnly, Category = "Stun")
    float CurrentStunCooldown = 0.0f;

    UFUNCTION(BlueprintImplementableEvent, Category = "Stun")
    void OnStunCooldownProgress(float Progress);

    void SpawnStunFlash();
    void ScheduleNextXylanShout();

    void PerformXylanShout();
    FTimerHandle XylanShoutTimerHandle;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Compass")
    UStaticMeshComponent* CompassArrowMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Xylan|Shout")
    float MinShoutInterval = 30.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Xylan|Shout")
    float MaxShoutInterval = 60.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Xylan|Shout")
    float ShoutAggroRadius = 2000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Xylan|Shout")
    float ShoutLoudness = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Xylan|Shout")
    TArray<USoundBase*> XylanShoutSounds;

    void SetBossRoomLocation(const FVector& Location);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash")
    UNiagaraSystem* FlashEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash")
    float FlashCooldown = 3.0f;
    ACTGCharacter(const FObjectInitializer& OfjInit);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flash", Transient)
    bool bIsFlashOnCooldown = false;

    virtual void Tick(float DeltaTime) override;
    UFUNCTION(BlueprintCallable, Category = "Chase")
    void SetIsChased(bool bChased);

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun")
    float StunDistance = 600.f;
    UPROPERTY(BlueprintReadWrite, Category = "Chase")
    bool bIsChased = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun")
    float StunConeAngle = 45.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun")
    float StunCooldown = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun")
    FTimerHandle StunCooldownTimer;

    void ResetStun();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    UAnimMontage* InteractMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    USoundBase* InteractSound;

    UFUNCTION(BlueprintCallable, Category = "Movenent")
    bool IsCharacterRunning() const;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    TArray<USoundBase*> CrouchFootstepSounds;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Footsteps")
    float CrouchFootstepLoudness = 0.4f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Footsteps")
    float WalkFootstepLoudness = 0.8f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Footsteps")
    float RunFootstepLoudness = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Footsteps")
    float CrouchFootstepRange = 1000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Footsteps")
    float WalkFootstepRange = 2000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound|Footsteps")
    float RunFootstepRange = 3000.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    TArray<USoundBase*> WalkFootstepSounds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    TArray<USoundBase*> RunFootstepSounds;
    void PlayFootstep();

protected:
    UPROPERTY()
    class ACTGPlayerState* CachedPlayerState = nullptr;
    UFUNCTION()
    void OnPointsChanged(ACTGPlayerState* PS, int32 NewPoints, int32 Delta);
    FVector BossRoomLocation = FVector::ZeroVector;
    bool bShowCompassArrow = false;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Compass", meta = (AllowPrivateAccess = "true"))
    class UArrowComponent* CompassArrow;

    FVector BossDoorLocation;

    int32 Score = 0;
    int32 CompassScoreThreshold = 2500;

    void UpdateCompass();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USpringArmComponent> SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UCameraComponent> CameraComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UCTGInteractionComponent> InteractionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UCTGStaminaComponent> StaminaComponent;

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
    TObjectPtr<UInputAction> SprintAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> InteractAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> StunAction;

    UPROPERTY(EditDefaultsOnly, Category = "Spawning")
    TSubclassOf<ACTGKsilanCharacter> KsilanClass;

    virtual void BeginPlay() override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    void StartCrouch(const FInputActionValue& Value);
    void StopCrouch(const FInputActionValue& Value);

    void PrimaryInteract();
    UFUNCTION(BlueprintCallable)
    void TryStunEnemies();

    virtual FVector GetPawnViewLocation() const override;

private:
    FTimerHandle FlashCooldownTimerHandle;
    bool WantsToSprint = false;
    bool IsMoving = false;

    void OnStartSprinting();
    void OnStopSprinting();

    void OnStaminaEmpty();

    void DestroyStunFlash(UPointLightComponent* Flash);

    void FindInteractionNotify();
    void OnInteractFinished(USkeletalMeshComponent* MeshComp);
};
