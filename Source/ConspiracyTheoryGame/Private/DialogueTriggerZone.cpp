// Team Development of a Conspiracy Theory Game for GameBOX.


#include "DialogueTriggerZone.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"

ADialogueTriggerZone::ADialogueTriggerZone()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
    RootComponent = TriggerZone;
    TriggerZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerZone->SetCollisionObjectType(ECC_WorldStatic);
    TriggerZone->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ADialogueTriggerZone::OnOverlapBegin);
}

void ADialogueTriggerZone::BeginPlay()
{
    Super::BeginPlay();
    ActiveWidget = nullptr;
}

void ADialogueTriggerZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ActiveWidget || !OtherActor) return;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (OtherActor != PlayerPawn) return;

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC || !DialogueWidgetClass) return;

    UUserWidget* Widget = CreateWidget<UUserWidget>(PC, DialogueWidgetClass);
    if (!Widget) return;

    Widget->AddToViewport();

    ActiveWidget = Widget;

    FTimerDelegate TimerDel;
    TimerDel.BindUFunction(this, FName("HideWidget"));

    GetWorldTimerManager().SetTimer(HideWidgetTimerHandle, TimerDel, WidgetDisplayTime, false);

    TriggerZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADialogueTriggerZone::HideWidget()
{
    if (ActiveWidget)
    {
        ActiveWidget->RemoveFromParent();
        ActiveWidget = nullptr;
    }
}