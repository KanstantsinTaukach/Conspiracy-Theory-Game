// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Enemy/EnemyCharacter.h"
#include "Enemy/EnemyAIController.h"
#include "Player/CTGPlayerState.h"
#include "Player/CTGCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Perception/AISense_Hearing.h"
#include "CTGGameInstance.h"

#include "DrawDebugHelpers.h"
#include "TimerManager.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    WeaponComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComponent"));
    WeaponComponent->SetupAttachment(GetMesh(), FName("LeftHandMiddlePoint"));
    WeaponComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    WeaponComponent->SetGenerateOverlapEvents(true);
    WeaponComponent->SetCollisionObjectType(ECC_WorldDynamic);
    WeaponComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

    PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
    PawnSensing->SightRadius = 350.f;
    PawnSensing->SetPeripheralVisionAngle(60.f);
    PawnSensing->HearingThreshold = 400.f;
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

    if (WeaponMesh && WeaponComponent)
    {
        WeaponComponent->SetStaticMesh(WeaponMesh);
    }

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

    if (WeaponComponent)
    {
        WeaponComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnWeaponOverlap);
        WeaponComponent->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::OnWeaponEndOverlap);
    }

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
    }
}
void AEnemyCharacter::OnSeePawn(APawn* Pawn)
{
    if (bIsChasing || bIsStunned) return;

    bIsChasing = true;
    bIsRunning = true;

    // Устанавливаем скорость погони
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
    }

    ACTGCharacter* CTGCharacter = Cast<ACTGCharacter>(Pawn);
    if (CTGCharacter)
    {
        CurrentTargetPlayer = CTGCharacter;
        CTGCharacter->SetIsChased(true);
    }
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

    if (CTGCharacter)
    {
        CTGCharacter->SetIsChased(true);
    }
}

void AEnemyCharacter::StartAttack()
{
    if (!AttackMontage)
    {
        UE_LOG(LogTemp, Error, TEXT("AttackMontage is NULL!"));
        return;
    }

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (!AnimInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("AnimInstance is NULL!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Playing AttackMontage"));
    AnimInstance->Montage_Play(AttackMontage);

    if (AttackSound)
    {

        if (!AttackAudioComponent || !AttackAudioComponent->IsPlaying())
        {
            AttackAudioComponent = UGameplayStatics::SpawnSoundAttached(AttackSound, GetRootComponent());
        }
    }
}

void AEnemyCharacter::OnHearNoise(APawn* InstigatorPawn, const FVector& Location, float Volume)
{
    UE_LOG(LogTemp, Error, TEXT("ENEMY HEARD NOISE"));
    if (!InstigatorPawn || InstigatorPawn == this || bIsChasing || bIsStunned)
    {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Enemy heard noise from %s at location %s with volume %.2f"), *GetNameSafe(InstigatorPawn),
        *Location.ToString(), Volume);

    AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
    if (!AIController) return;

    // Начинаем погоню
    bIsChasing = true;
    bIsRunning = true;

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
    }

    CurrentTargetPlayer = Cast<ACTGCharacter>(InstigatorPawn);
    if (CurrentTargetPlayer)
    {
        CurrentTargetPlayer->SetIsChased(true);
    }

    AIController->StartChasing(InstigatorPawn);

    if (ChaseSound && ChaseAudio && !ChaseAudio->IsPlaying())
    {
        ChaseAudio->SetSound(ChaseSound);
        ChaseAudio->Play();
    }

    StopPatrolSound();
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

void AEnemyCharacter::Stun()
{
    if (bIsStunned) return;

    bIsStunned = true;

    AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
    if (AIController)
    {
        AIController->ChaseTarget = nullptr;
        AIController->bIsChasing = false;

        APawn* TargetPawn = AIController ? Cast<APawn>(AIController->GetFocusActor()) : nullptr;
        if (TargetPawn)
        {
            ACTGCharacter* PlayerCharacter = Cast<ACTGCharacter>(TargetPawn);
            if (PlayerCharacter)
            {
                PlayerCharacter->SetIsChased(false);
            }
        }

        StopChaseSound();
        bIsRunning = false;
        bIsChasing = false;

        AIController->StopMovement();
        AIController->ClearFocus(EAIFocusPriority::Gameplay);

        AIController->GetWorldTimerManager().ClearTimer(AIController->LoseTargetTimerHandle);
        AIController->GetWorldTimerManager().ClearTimer(AIController->ReturnToPatrolTimerHandle);

    }


    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
    }

    if (StunMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            AnimInstance->Montage_Play(StunMontage);
        }
    }

    if (AIController)
    {
        AIController->GetWorldTimerManager().SetTimer(
            AIController->ReturnToPatrolTimerHandle,
            [this, AIController]()
            {
                bIsStunned = false;
                AIController->ResumePatrol();


                if (GetCharacterMovement())
                {
                    GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
                }
            },
            StunDuration, false);
    }
}

void AEnemyCharacter::StopChaseSound()
{
    if (ChaseAudio && ChaseAudio->IsPlaying())
    {
        ChaseAudio->Stop();
        bIsRunning = false;
    }
}

void AEnemyCharacter::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor) return;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (OtherActor == PlayerPawn)
    {
        if (!bIsOverlappingPlayer)
        {
            bIsOverlappingPlayer = true;

            StartAttack();

            GetWorldTimerManager().SetTimer(
                OverlapCatchTimerHandle, this, &AEnemyCharacter::OnInitialCatchTimerExpired, OverlapTimeToCatch, false);
        }
    }
}

void AEnemyCharacter::OnWeaponEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (OtherActor == PlayerPawn)
    {
        bIsOverlappingPlayer = false;

        GetWorldTimerManager().ClearTimer(OverlapCatchTimerHandle);
    }
}
void AEnemyCharacter::OnInitialCatchTimerExpired()
{

    GetWorldTimerManager().SetTimer(OverlapCatchTimerHandle, this, &AEnemyCharacter::HandlePlayerCaught, 0.5f, false);
}

void AEnemyCharacter::HandlePlayerCaught()
{
    if (!bIsOverlappingPlayer)
    {
        return;
    }

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn)
    {
        return;
    }

    APlayerController* PC = Cast<APlayerController>(PlayerPawn->GetController());
    if (!PC)
    {
        return;
    }

    UUserWidget* CatchWidget = nullptr;
    if (CatchWidgetClass)
    {
        CatchWidget = CreateWidget<UUserWidget>(PC, CatchWidgetClass);
        if (CatchWidget)
        {
            CatchWidget->AddToViewport();
        }
    }

    PC->SetIgnoreMoveInput(true);
    PC->SetIgnoreLookInput(true);

    ACTGCharacter* CTGCharacter = Cast<ACTGCharacter>(PlayerPawn);
    if (CTGCharacter)
    {
        CTGCharacter->GetCharacterMovement()->DisableMovement();

        UAnimInstance* AnimInstance = CTGCharacter->GetMesh()->GetAnimInstance();
        if (AnimInstance && CTGCharacter->LoseMontage)
        {
            AnimInstance->Montage_Play(CTGCharacter->LoseMontage);
        }
    }

    const auto CTGGameInstance = GetWorld()->GetGameInstance<UCTGGameInstance>();
    if (CTGGameInstance)
    {
        ACTGPlayerState* PS = Cast<ACTGPlayerState>(PC->PlayerState);
        if (PS)
        {
            PS->RemovePoints(PS->GetPoints() / 2);
            CTGGameInstance->SetPlayerScore(PS->GetPoints());
        }

        FName LevelToOpen = CTGGameInstance->GetStartupLevelName();

        FTimerHandle DelayHandle;
        FTimerDelegate Delegate;
        Delegate.BindUFunction(this, FName("OpenLevelAfterDelay"), PC, LevelToOpen);
        GetWorldTimerManager().SetTimer(DelayHandle, Delegate, CatchWidgetDisplayTime, false);
    }
}

void AEnemyCharacter::OpenLevelAfterDelay(APlayerController* PC, FName LevelName)
{
    if (PC)
    {
        PC->SetIgnoreMoveInput(false);
        PC->SetIgnoreLookInput(false);
    }

    UGameplayStatics::OpenLevel(this, LevelName);
}

void AEnemyCharacter::PlayFootstep(bool bRunning)
{
    const TArray<USoundBase*>& FootstepSounds = bRunning ? RunFootstepSounds : WalkFootstepSounds;

    if (FootstepSounds.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, FootstepSounds.Num() - 1);
        USoundBase* SelectedSound = FootstepSounds[Index];

        if (SelectedSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, SelectedSound, GetActorLocation());

            UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), bIsRunning ? 1.0f : 0.4f, this);
        }
    }
}