// Team Development of a Conspiracy Theory Game for GameBOX.


#include "Enemy/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EnemyCharacter.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentPatrolIndex = 0;
    ChaseTarget = nullptr;
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> AllPoints;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Patrol"), AllPoints);

    if (AllPoints.Num() == 0)
    {
    }
    else
    {
        PatrolPoints = AllPoints;

        MoveToNextPatrolPoint();
    }
}

void AEnemyAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!ChaseTarget)
    {

        if (PatrolPoints.Num() > 0 && GetPathFollowingComponent()->GetStatus() != EPathFollowingStatus::Moving &&
            !GetWorld()->GetTimerManager().IsTimerActive(ReturnToPatrolTimerHandle))
        {
            MoveToNextPatrolPoint();
        }
        return;
    }

    MoveToActor(ChaseTarget);
    float DistanceToTarget = FVector::Dist(ChaseTarget->GetActorLocation(), GetPawn()->GetActorLocation());

    if (DistanceToTarget < 150.0f)
    {
        AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(GetPawn());
        if (EnemyChar)
        {
            EnemyChar->StartAttack();
        }
    }

    const FVector TargetLocation = ChaseTarget->GetActorLocation();
    const FVector MyLocation = GetPawn()->GetActorLocation();
    DistanceToTarget = FVector::Distance(TargetLocation, MyLocation);

    const bool bCanSee = LineOfSightTo(ChaseTarget);
    const bool bCanHear = DistanceToTarget < HearingRadius;

    if (bCanSee || bCanHear)
    {
        GetWorld()->GetTimerManager().ClearTimer(LoseTargetTimerHandle);
        bIsChasing = true;
    }
    else if (bIsChasing)
    {
        if (!GetWorld()->GetTimerManager().IsTimerActive(LoseTargetTimerHandle))
        {
            GetWorld()->GetTimerManager().SetTimer(LoseTargetTimerHandle, this, &AEnemyAIController::LoseTarget, TimeToLoseTarget, false);
        }
    }
}

void AEnemyAIController::MoveToNextPatrolPoint()
{
    if (PatrolPoints.Num() == 0)
    {

        return;
    }

    int32 NextIndex;
    do
    {
        NextIndex = FMath::RandRange(0, PatrolPoints.Num() - 1);
    } while (NextIndex == CurrentPatrolIndex && PatrolPoints.Num() > 1);

    CurrentPatrolIndex = NextIndex;

    AActor* NextPoint = PatrolPoints[CurrentPatrolIndex];
    if (NextPoint)
    {
        MoveToActor(NextPoint);
    }
}

void AEnemyAIController::StartChasing(APawn* Target)
{
    if (!Target || ChaseTarget == Target) return;

    ChaseTarget = Target;
    bIsChasing = true;


    GetWorld()->GetTimerManager().ClearTimer(LoseTargetTimerHandle);


    GetWorld()->GetTimerManager().SetTimer(LoseTargetTimerHandle, this, &AEnemyAIController::LoseTarget, TimeToLoseTarget, false);
}

void AEnemyAIController::LoseTarget()
{
    if (!ChaseTarget) return;

    ChaseTarget = nullptr;
    bIsChasing = false;

    AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(GetPawn());
    if (EnemyChar)
    {
        EnemyChar->bIsChasing = false;
        EnemyChar->StopChaseSound();
        EnemyChar->StartPatrolSound();


        StopMovement();


        if (EnemyChar->LostTargetMontage)
        {
            UAnimInstance* AnimInstance = EnemyChar->GetMesh()->GetAnimInstance();
            if (AnimInstance)
            {
                AnimInstance->Montage_Play(EnemyChar->LostTargetMontage);
            }
        }
    }


    GetWorld()->GetTimerManager().SetTimer(ReturnToPatrolTimerHandle, this, &AEnemyAIController::ResumePatrol, TimeToResumePatrol, false);
}

void AEnemyAIController::ResumePatrol()
{

    MoveToNextPatrolPoint();
}


