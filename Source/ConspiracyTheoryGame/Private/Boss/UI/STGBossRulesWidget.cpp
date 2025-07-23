// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/UI/STGBossRulesWidget.h"
#include "Components/Button.h"
#include "CTGRhythmGameModeBase.h"

void USTGBossRulesWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartBattleButton)
    {
        StartBattleButton->OnClicked.AddDynamic(this, &USTGBossRulesWidget::OnStartBattle);
    }
}

void USTGBossRulesWidget::OnStartBattle()
{
    if (!GetWorld()) return;

    auto* GameMode = Cast<ACTGRhythmGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->StartBattleWithBoss();
    }
}