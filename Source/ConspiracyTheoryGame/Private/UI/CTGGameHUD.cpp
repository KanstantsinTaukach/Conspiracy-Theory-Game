// Team Development of a Conspiracy Theory Game for GameBOX.

#include "UI/CTGGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "CTGGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogCTGGameHUD, All, All);

void ACTGGameHUD::BeginPlay() 
{
    Super::BeginPlay();

    auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->AddToViewport();
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

void ACTGGameHUD::OnMatchStateChanged(CTGMatchState State) 
{
    UE_LOG(LogCTGGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}
