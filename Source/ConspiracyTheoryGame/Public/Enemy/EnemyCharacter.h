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

    /** Sound used for patrol loop */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* PatrolLoopSound;

    /** Sound used once at the start of chase */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* ChaseSound;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    UAudioComponent* ChaseAudio;

    void StopChaseSound();

    void StopPatrolSound();
    void StartPatrolSound();
};
