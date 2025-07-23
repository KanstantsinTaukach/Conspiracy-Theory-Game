// Team Development of a Conspiracy Theory Game for GameBOX.

#include "UI/CTGPlayerWinWidget.h"
#include "Components/Button.h"
#include "CTGGameInstance.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogCTGPlayerWinWidget, All, All)

void UCTGPlayerWinWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (RestartGameButton)
    {
        RestartGameButton->OnClicked.AddDynamic(this, &UCTGPlayerWinWidget::OnRestartGame);
    }

        if (RestartLevelButton)
    {
            RestartLevelButton->OnClicked.AddDynamic(this, &UCTGPlayerWinWidget::OnRestartLevel);
    }
}

void UCTGPlayerWinWidget::OnRestartLevel()
{
    if (!GetWorld()) return;

    const auto CTGGameInstance = GetWorld()->GetGameInstance<UCTGGameInstance>();
    if (!CTGGameInstance) return;

    if (CTGGameInstance->GetBossLevelName().IsNone())
    {
        UE_LOG(LogCTGPlayerWinWidget, Error, TEXT("Boss level name is NONE"));
        return;
    }

    UGameplayStatics::OpenLevel(this, CTGGameInstance->GetBossLevelName());
}

void UCTGPlayerWinWidget::OnRestartGame()
{
    if (!GetWorld()) return;

    const auto CTGGameInstance = GetWorld()->GetGameInstance<UCTGGameInstance>();
    if (!CTGGameInstance) return;

    if (CTGGameInstance->GetStartupLevelName().IsNone())
    {
        UE_LOG(LogCTGPlayerWinWidget, Error, TEXT("Start level name is NONE"));
        return;
    }

    UGameplayStatics::OpenLevel(this, CTGGameInstance->GetStartupLevelName());
}
