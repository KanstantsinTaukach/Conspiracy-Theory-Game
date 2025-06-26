// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Sound/SoundBase.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class CONSPIRACYTHEORYGAME_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyAIController();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY()
    TArray<AActor*> PatrolPoints;

    int32 CurrentPatrolIndex;

    void MoveToNextPatrolPoint();

    UPROPERTY()
    APawn* ChaseTarget;

    void LoseTarget();


public:
    UPROPERTY(BlueprintReadOnly, Category = "AI")
    bool bIsChasing = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float TimeToLoseTarget = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float TimeToResumePatrol = 10.0f;

    FTimerHandle LoseTargetTimerHandle;
    FTimerHandle ReturnToPatrolTimerHandle;

    void StartChasing(APawn* Target);
    void ResumePatrol();
    /** Radius of visual detection */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
    float SightRadius = 1000.f;

    /** Radius of hearing detection */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
    float HearingRadius = 600.f;

    /** Sound played when chase starts (played at controlled pawn's location) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* ChaseSound;
};
