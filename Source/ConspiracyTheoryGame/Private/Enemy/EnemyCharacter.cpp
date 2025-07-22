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

    AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));


    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
    HearingConfig->HearingRange = HearingRadius;  
     HearingConfig->LoSHearingRange = HearingRadius * 1.2f;

    HearingConfig->bUseLoSHearing = true;     
    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

    AIPerceptionComp->ConfigureSense(*HearingConfig);
    AIPerceptionComp->SetDominantSense(HearingConfig->GetSenseImplementation());

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

void AEnemyCharacter::StartStunCooldown()
{
    CurrentStunCooldown = 0.0f;
    StunCooldownProgress = 0.0f;

    GetWorldTimerManager().SetTimer(StunCooldownTimerHandle, this, &AEnemyCharacter::UpdateStunCooldown, 0.05f, true);
}

void AEnemyCharacter::UpdateStunCooldown()
{
    CurrentStunCooldown += 0.05f;

    float Progress = FMath::Clamp(CurrentStunCooldown / StunCooldown, 0.0f, 1.0f);
    StunCooldownProgress = Progress;

    OnStunCooldownProgress(Progress); 

    if (CurrentStunCooldown >= StunCooldown)
    {
        bCanStun = true;
        GetWorldTimerManager().ClearTimer(StunCooldownTimerHandle);
    }
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (AIPerceptionComp)
    {
        AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AEnemyCharacter::OnPerceptionUpdated);
    }
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
void AEnemyCharacter::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    for (AActor* Actor : UpdatedActors)
    {
        FActorPerceptionBlueprintInfo Info;
        if (AIPerceptionComp->GetActorsPerception(Actor, Info))
        {
            for (FAIStimulus Stimulus : Info.LastSensedStimuli)
            {
                if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
                {
                    if (Stimulus.WasSuccessfullySensed())
                    {
                        // Обрабатываем услышанный звук
                        FVector SoundLocation = Stimulus.StimulusLocation;
                        float SoundStrength = Stimulus.Strength;

                        UE_LOG(
                            LogTemp, Warning, TEXT("1"), *SoundLocation.ToString(), SoundStrength);

                        // Проверяем, что это игрок
                        ACTGCharacter* Player = Cast<ACTGCharacter>(Actor);
                        if (Player && !bIsChasing && !bIsStunned)
                        {
                            // Начинаем погоню
                            AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
                            if (AIController)
                            {
                                AIController->StartChasing(Player);
                                bIsChasing = true;

                                if (GetCharacterMovement())
                                {
                                    GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
                                }

                                if (ChaseSound && ChaseAudio && !ChaseAudio->IsPlaying())
                                {
                                    ChaseAudio->SetSound(ChaseSound);
                                    ChaseAudio->Play();
                                }
                                StopPatrolSound();
                            }
                        }
                    }
                }
            }
        }
    }
}
void AEnemyCharacter::OnSeePawn(APawn* Pawn)
{
    if (bIsChasing || bIsStunned) return;

    ACTGCharacter* CTGCharacter = Cast<ACTGCharacter>(Pawn);
    if (!CTGCharacter) return;

    // Устанавливаем цель и включаем преследование
    CurrentTargetPlayer = CTGCharacter;
    CTGCharacter->SetIsChased(true);  // Явно устанавливаем состояние

    bIsChasing = true;
    bIsRunning = true;
    if (bCanPlayChaseSound)
    {
        PlayRandomChaseStartSound();
    }

    // Устанавливаем скорость погони
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
    }


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
    if (bIsStunned ) return;

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

    if (AnimInstance && AttackMontage)
    {

        if (!AnimInstance->Montage_IsPlaying(AttackMontage))
        {
            AnimInstance->Montage_Play(AttackMontage);

            if (AttackSound)
            {
                UGameplayStatics::SpawnSoundAttached(AttackSound, GetRootComponent());
            }
        }
    }
}

void AEnemyCharacter::OnHearNoise(APawn* InstigatorPawn, const FVector& Location, float Volume)
{
    if (!InstigatorPawn || InstigatorPawn == this || bIsChasing || bIsStunned) return;

    ACTGCharacter* CTGCharacter = Cast<ACTGCharacter>(InstigatorPawn);
    if (!CTGCharacter) return;
    if (bCanPlayChaseSound)
    {
        PlayRandomChaseStartSound();
    }

    CurrentTargetPlayer = CTGCharacter;
    CTGCharacter->SetIsChased(true);  

    bIsChasing = true;
    bIsRunning = true;

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
    if (bIsStunned || !bCanStun) return;

    // Clear chase state from current target
    if (CurrentTargetPlayer)
    {
        CurrentTargetPlayer->SetIsChased(false);
    }

    // Set stunned state
    bIsStunned = true;
    bCanStun = false;
    CurrentStunCooldown = 0.0f;
    StunCooldownProgress = 0.0f;

    // Handle AI controller behavior
    AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
    if (AIController)
    {
        AIController->ChaseTarget = nullptr;
        AIController->bIsChasing = false;

        // Clear focus from any target
        APawn* TargetPawn = Cast<APawn>(AIController->GetFocusActor());
        if (TargetPawn)
        {
            ACTGCharacter* PlayerCharacter = Cast<ACTGCharacter>(TargetPawn);
            if (PlayerCharacter)
            {
                PlayerCharacter->SetIsChased(false);
            }
        }

        // Stop chase behavior
        StopChaseSound();
        bIsRunning = false;
        bIsChasing = false;

        AIController->StopMovement();
        AIController->ClearFocus(EAIFocusPriority::Gameplay);

        // Clear any active timers
        AIController->GetWorldTimerManager().ClearTimer(AIController->LoseTargetTimerHandle);
        AIController->GetWorldTimerManager().ClearTimer(AIController->ReturnToPatrolTimerHandle);
    }

    // Play stun sound (removed VirtualizeWhenSilent check)
    if (LostTargetSound && LostTargetSound->IsValidLowLevel())
    {
        UGameplayStatics::PlaySoundAtLocation(this, LostTargetSound, GetActorLocation());
    }

    // Reset movement speed
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
    }

    // Play stun animation
    if (StunMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            AnimInstance->Montage_Play(StunMontage);
        }
    }

    // Set up resume patrol after stun duration
    /*if (AIController)
    {
        FTimerDelegate ResumePatrolDelegate;
        ResumePatrolDelegate.BindLambda(
            [this]()
            {
                bIsStunned = false;

                AEnemyAIController* RefreshedController = Cast<AEnemyAIController>(GetController());
                if (RefreshedController)
                {
                    RefreshedController->ResumePatrol();
                }

                if (GetCharacterMovement())
                {
                    GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
                }
            });

        AIController->GetWorldTimerManager().SetTimer(AIController->ReturnToPatrolTimerHandle, ResumePatrolDelegate, StunDuration, false);
    }*/


    StartStunCooldown();
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

    GetWorldTimerManager().SetTimer(OverlapCatchTimerHandle, this, &AEnemyCharacter::HandlePlayerCaught, 0.1f, false);
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

    const auto* CTGGameInstance = GetWorld()->GetGameInstance<UCTGGameInstance>();
    if (CTGGameInstance)
    {
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

    auto* CTGGameInstance = GetWorld()->GetGameInstance<UCTGGameInstance>();
    if (CTGGameInstance)
    {
        ACTGPlayerState* PS = Cast<ACTGPlayerState>(PC->PlayerState);
        if (PS)
        {
            PS->RemovePoints(PS->GetPoints() / 2);
            CTGGameInstance->SetPlayerScore(PS->GetPoints());
        }
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

void AEnemyCharacter::PlayRandomChaseStartSound()
{
    if (!bCanPlayChaseSound || ChaseStartSounds.Num() == 0) return;

    int32 RandomIndex = FMath::RandRange(0, ChaseStartSounds.Num() - 1);
    USoundBase* SelectedSound = ChaseStartSounds[RandomIndex];

    if (SelectedSound && SelectedSound->IsValidLowLevel())
    {
        bCanPlayChaseSound = false;

        // Play the sound regardless of virtualization settings
        float SoundDuration = SelectedSound->GetDuration();

        UGameplayStatics::SpawnSoundAtLocation(
            this, SelectedSound, GetActorLocation(), FRotator::ZeroRotator, 1.0f, 1.0f, 0.0f, nullptr, nullptr, false);

        GetWorld()->GetTimerManager().SetTimer(
            ChaseSoundTimerHandle, this, &AEnemyCharacter::ResetChaseSound, FMath::Max(0.1f, SoundDuration), false);
    }
    else
    {
        bCanPlayChaseSound = true;
    }
}
void AEnemyCharacter::ResetChaseSound()
{
    bCanPlayChaseSound = true;
}