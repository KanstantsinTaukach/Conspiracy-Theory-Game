// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class CONSPIRACYTHEORYGAME_API AEnemyCharacter : public ACharacter
{
    GENERATED_BODY()
private:
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|State")
    bool bIsStunned = false;

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


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Level")
    FName LevelToOpen;
    UFUNCTION()
    void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    void StartAttack();

    UFUNCTION(BlueprintCallable)
    void Stun();

    void StopChaseSound();

    void StopPatrolSound();
    void StartPatrolSound();
};
