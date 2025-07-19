// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Sound/SoundBase.h"
#include "CTGCharacter.h"
#include "Components/AudioComponent.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class CONSPIRACYTHEORYGAME_API AEnemyCharacter : public ACharacter
{
    GENERATED_BODY()
private:
    FTimerHandle OverlapCatchTimerHandle;

    UFUNCTION()
    void OpenLevelAfterDelay(APlayerController* PC, FName LevelName);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stun")
    float StunCooldown = 5.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stun")
    bool bCanStun = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stun")
    bool bIsStunned = false;

    UPROPERTY(BlueprintReadOnly, Category = "Stun")
    float CurrentStunCooldown = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Stun")
    float StunCooldownProgress = 0.0f;  // <- эту ты будешь брать в UMG

    FTimerHandle StunCooldownTimerHandle;

    UFUNCTION(BlueprintImplementableEvent, Category = "Stun")
    void OnStunCooldownProgress(float Progress);

    // Функции
    void StartStunCooldown();
    void UpdateStunCooldown();

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float PatrolSpeed = 200.f;

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float ChaseSpeed = 600.f;

    UPROPERTY()
    UAudioComponent* AttackAudioComponent;

    bool bIsRunning;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Footstep")
    TArray<USoundBase*> WalkFootstepSounds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Footstep")
    TArray<USoundBase*> RunFootstepSounds;

    UFUNCTION(BlueprintCallable)
    void PlayFootstep(bool bIsRunning);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* LostTargetSound;

    UPROPERTY()
    ACTGCharacter* CurrentTargetPlayer = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* AttackSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> CatchWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture")
    float OverlapTimeToCatch = 0.5f;

    float CurrentOverlapTime = 0.0f;

    bool bIsOverlappingPlayer = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    bool bIsChasing = false;
    AEnemyCharacter();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UPawnSensingComponent* PawnSensing;

    UFUNCTION()
    void OnSeePawn(APawn* Pawn);

    UFUNCTION()
    void OnHearNoise(APawn* InstigatorPawn, const FVector& Location, float Volume);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Animation")
    UAnimMontage* LostTargetMontage;

    /** Looping patrol audio component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    UAudioComponent* PatrolLoopAudio;


    /** Sound used for patrol loop */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* PatrolLoopSound;

    /** Sound used once at the start of chase */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* ChaseSound;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    UAudioComponent* ChaseAudio;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|State")
    float StunDuration = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Animation")
    UAnimMontage* StunMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Weapon")
    UStaticMesh* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Weapon")
    UStaticMeshComponent* WeaponComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Animation")
    UAnimMontage* AttackMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    float CatchWidgetDisplayTime = 2.0f;

    UFUNCTION()
    void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    void OnInitialCatchTimerExpired();

    UFUNCTION()
    void OnWeaponEndOverlap(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void HandlePlayerCaught();

    void StartAttack();

    UFUNCTION(BlueprintCallable)
    void Stun();

    void StopChaseSound();

    void StopPatrolSound();
    void StartPatrolSound();
};
