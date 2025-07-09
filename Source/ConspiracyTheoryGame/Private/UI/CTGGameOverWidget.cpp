// Team Development of a Conspiracy Theory Game for GameBOX.

#include "UI/CTGGameOverWidget.h"
#include "Components/Button.h"
#include "CTGGameInstance.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogCTGGameOverWidget, All, All)

void UCTGGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (RestartLevelButton)
    {
        RestartLevelButton->OnClicked.AddDynamic(this, &UCTGGameOverWidget::OnRestartLevel);
    }
}

void UCTGGameOverWidget::OnRestartLevel() 
{
    if (!GetWorld()) return;

    const auto CTGGameInstance = GetWorld()->GetGameInstance<UCTGGameInstance>();
    if (!CTGGameInstance) return;

    if (CTGGameInstance->GetStartupLevelName().IsNone())
    {
        UE_LOG(LogCTGGameOverWidget, Error, TEXT("Level name is NONE"));
        return;
    }

    UGameplayStatics::OpenLevel(this, CTGGameInstance->GetStartupLevelName());
}
