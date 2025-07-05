// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/UI/CTGBossGameplayWidget.h"
#include "Boss/CTGRhythmGameModeBase.h"
#include "Boss/RhythmMechanics/CTGVisualCharacter.h"
#include "Components/ProgressBar.h"

float UCTGBossGameplayWidget::GetPlayerHealthPercent() const
{
    if (!GetWorld()) return 0.0f;

    if (auto* GameMode = Cast<ACTGRhythmGameModeBase>(GetWorld()->GetAuthGameMode()))
    {
        const auto* Player = GameMode->GetVisualPlayerCharacter();
        if (Player)
        {
            return Player->GetHealthPercent();
        }
    }

    return 0.0f;
}

float UCTGBossGameplayWidget::GetBossHealthPercent() const
{
    if (!GetWorld()) return 0.0f;

    if (auto* GameMode = Cast<ACTGRhythmGameModeBase>(GetWorld()->GetAuthGameMode()))
    {
        const auto* Boss = GameMode->GetVisualBossCharacter();
        if (Boss)
        {
            return Boss->GetHealthPercent();
        }
    }

    return 0.0f;
}