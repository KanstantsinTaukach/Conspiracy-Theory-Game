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

void AEnemyAIController::MoveTowardsPlayer()
{
    if (!ChaseTarget || !GetPawn()) return;

    FVector Direction = (ChaseTarget->GetActorLocation() - GetPawn()->GetActorLocation()).GetSafeNormal();

    FVector NewLocation = GetPawn()->GetActorLocation() + Direction * ManualMoveSpeed * GetWorld()->GetDeltaSeconds();

    FHitResult Hit;
    GetPawn()->SetActorLocation(NewLocation, true, &Hit);

    // Если столкнулся с препятствием — можно отключить ручное движение
    if (Hit.bBlockingHit)
    {
        GetWorld()->GetTimerManager().ClearTimer(ManualMoveTimerHandle);
    }
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

    const FVector TargetLocation = ChaseTarget->GetActorLocation();
    const FVector MyLocation = GetPawn()->GetActorLocation();
    float DistanceToTarget = FVector::Distance(TargetLocation, MyLocation);


    EPathFollowingRequestResult::Type Result = MoveToActor(ChaseTarget);

    if (Result == EPathFollowingRequestResult::Failed)
    {
        if (!GetWorld()->GetTimerManager().IsTimerActive(ManualMoveTimerHandle))
        {
            GetWorld()->GetTimerManager().SetTimer(ManualMoveTimerHandle, this, &AEnemyAIController::MoveTowardsPlayer, 0.02f, true);
        }
    }
    else
    {

        GetWorld()->GetTimerManager().ClearTimer(ManualMoveTimerHandle);
    }


    if (DistanceToTarget < 50.0f)
    {
        AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(GetPawn());
        if (EnemyChar)
        {
            EnemyChar->StartAttack();
        }
    }


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

    if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetPawn()))
    {
        if (ACTGCharacter* Player = Cast<ACTGCharacter>(Target))
        {
            Player->SetIsChased(true);
            Enemy->CurrentTargetPlayer = Player;
        }
    }

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
    StopMovement();
    bIsChasing = false;

    if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetPawn()))
    {
        Enemy->bIsChasing = false;

        if (Enemy->CurrentTargetPlayer)
        {
            Enemy->CurrentTargetPlayer->SetIsChased(false);
            Enemy->CurrentTargetPlayer = nullptr;
        }
        if (Enemy->LostTargetSound)
        {
            UGameplayStatics::PlaySoundAtLocation(Enemy, Enemy->LostTargetSound, Enemy->GetActorLocation());
        }
    }

    MoveToNextPatrolPoint();
}


