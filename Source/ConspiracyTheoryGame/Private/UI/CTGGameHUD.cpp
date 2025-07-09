// Team Development of a Conspiracy Theory Game for GameBOX.

#include "UI/CTGGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "CTGGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogCTGGameHUD, All, All);

void ACTGGameHUD::BeginPlay() 
{
    Super::BeginPlay();

    GameWidgets.Add(ECTGMatchState::InProgress, CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass));
    GameWidgets.Add(ECTGMatchState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass));
    //GameWidgets.Add(ECTGMatchState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));

    for (auto GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if (!GameWidget) continue;

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GetWorld())
    {
        const auto GameMode = Cast<ACTGGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ACTGGameHUD::OnMatchStateChanged);
        }
    }
}

void ACTGGameHUD::OnMatchStateChanged(ECTGMatchState State)
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
