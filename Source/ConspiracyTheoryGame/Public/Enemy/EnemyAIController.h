// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Sound/SoundBase.h"
#include "EnemyAIController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartChasingSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoseTargetSignature);

UCLASS()
class CONSPIRACYTHEORYGAME_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyAIController();
    FTimerHandle ManualMoveTimerHandle;
    float ManualMoveSpeed = 450.f;
    void MoveTowardsPlayer();

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnStartChasingSignature OnStartChasing;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLoseTargetSignature OnLoseTarget;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;



private:
    UPROPERTY()
    TArray<AActor*> PatrolPoints;

    int32 CurrentPatrolIndex;

    void MoveToNextPatrolPoint();

    void LoseTarget();

public:
    UPROPERTY()
    APawn* ChaseTarget;
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
