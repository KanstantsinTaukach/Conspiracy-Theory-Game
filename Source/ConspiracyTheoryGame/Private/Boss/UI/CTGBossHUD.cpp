// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/UI/CTGBossHUD.h"
#include "Blueprint/UserWidget.h"
#include "Boss/CTGRhythmGameModeBase.h"

void ACTGBossHUD::BeginPlay() 
{
    Super::BeginPlay();

    GameWidgets.Add(ECTGMatchState::FightingWithBoss, CreateWidget<UUserWidget>(GetWorld(), BossGameplayWidgetClass));
    GameWidgets.Add(ECTGMatchState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));
    GameWidgets.Add(ECTGMatchState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass));

    for (auto GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if (!GameWidget) continue;

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GetWorld())
    {
        const auto GameMode = Cast<ACTGRhythmGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ACTGBossHUD::OnMatchStateChanged);
        }
    }
}

void ACTGBossHUD::OnMatchStateChanged(ECTGMatchState State)
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GameWidgets.Contains(State))
    {
        CurrentWidget = GameWidgets[State];
    }

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
    }
}