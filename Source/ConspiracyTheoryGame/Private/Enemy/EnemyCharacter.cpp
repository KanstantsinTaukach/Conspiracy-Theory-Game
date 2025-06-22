// Team Development of a Conspiracy Theory Game for GameBOX.


#include "Enemy/EnemyCharacter.h"
#include "Enemy/EnemyAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;


    PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
    PawnSensing->SightRadius = 350.f;
    PawnSensing->SetPeripheralVisionAngle(60.f);
    PawnSensing->HearingThreshold = 150.f;
    PawnSensing->bSeePawns = true;
    PawnSensing->bHearNoises = true;

    PatrolLoopAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("PatrolLoopAudio"));
    PatrolLoopAudio->SetupAttachment(RootComponent);
    PatrolLoopAudio->bAutoActivate = false;

    ChaseAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("ChaseAudio"));
    ChaseAudio->SetupAttachment(RootComponent);
    ChaseAudio->bAutoActivate = false;
}

void AEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PawnSensing)
    {
        FVector Location = GetActorLocation();
        FVector Forward = GetActorForwardVector();

        DrawDebugCone(GetWorld(), Location, Forward, PawnSensing->SightRadius,
            FMath::DegreesToRadians(PawnSensing->GetPeripheralVisionAngle()),
            FMath::DegreesToRadians(PawnSensing->GetPeripheralVisionAngle()), 12, FColor::Green, false, -1.0f, 0, 1.0f);

        DrawDebugSphere(GetWorld(), Location, PawnSensing->HearingThreshold, 16, FColor::Blue, false, -1.0f, 0, 1.0f);
    }
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (PawnSensing)
    {
        PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSeePawn);
        PawnSensing->OnHearNoise.AddDynamic(this, &AEnemyCharacter::OnHearNoise);
    }

    if (PatrolLoopSound && PatrolLoopAudio)
    {
        PatrolLoopAudio->SetSound(PatrolLoopSound);
        PatrolLoopAudio->Play();
    }
}

void AEnemyCharacter::OnSeePawn(APawn* Pawn)
{
    if (bIsChasing) return;  

    bIsChasing = true;

    AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
    if (AIController)
    {
        AIController->StartChasing(Pawn);

        if (ChaseSound && ChaseAudio && !ChaseAudio->IsPlaying())
        {
            ChaseAudio->SetSound(ChaseSound);
            ChaseAudio->Play();
        }

        StopPatrolSound();
    }
}

void AEnemyCharacter::OnHearNoise(APawn* InstigatorPawn, const FVector& Location, float Volume)
{
    if (bIsChasing) return;

    bIsChasing = true;

    AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
    if (AIController && InstigatorPawn)
    {
        AIController->StartChasing(InstigatorPawn);

        if (ChaseSound && ChaseAudio && !ChaseAudio->IsPlaying())
        {
            ChaseAudio->SetSound(ChaseSound);
            ChaseAudio->Play();
        }

        StopPatrolSound();
    }
}
void AEnemyCharacter::StopPatrolSound()
{
    if (PatrolLoopAudio && PatrolLoopAudio->IsPlaying())
    {
        PatrolLoopAudio->Stop();
    }
}

void AEnemyCharacter::StartPatrolSound()
{
    if (PatrolLoopSound && PatrolLoopAudio && !PatrolLoopAudio->IsPlaying())
    {
        PatrolLoopAudio->SetSound(PatrolLoopSound);
        PatrolLoopAudio->Play();
    }
}
void AEnemyCharacter::StopChaseSound()
{
    if (ChaseAudio && ChaseAudio->IsPlaying())
    {
        ChaseAudio->Stop();
    }
}

