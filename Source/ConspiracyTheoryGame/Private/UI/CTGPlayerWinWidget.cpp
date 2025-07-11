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
