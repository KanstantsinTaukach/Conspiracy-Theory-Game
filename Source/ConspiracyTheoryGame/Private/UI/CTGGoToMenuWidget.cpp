// Team Development of a Conspiracy Theory Game for GameBOX.

#include "UI/CTGGoToMenuWidget.h"
#include "Components/Button.h"
#include "CTGGameInstance.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogCTGGoToMenuWidget, All, All);

void UCTGGoToMenuWidget::NativeOnInitialized() 
{
    Super::NativeOnInitialized();

    if (GoToMenuButton)
    {
        GoToMenuButton->OnClicked.AddDynamic(this, &UCTGGoToMenuWidget::OnGoToMenu);
    }
}

void UCTGGoToMenuWidget::OnGoToMenu()
{
    if (!GetWorld()) return;

    const auto CTGGameInstance = GetWorld()->GetGameInstance<UCTGGameInstance>();
    if (!CTGGameInstance) return;

    if (CTGGameInstance->GetMenuLevelName().IsNone())
    {
        UE_LOG(LogCTGGoToMenuWidget, Error, TEXT("Menu level name is NONE"));
        return;
    }
    UGameplayStatics::OpenLevel(this, CTGGameInstance->GetMenuLevelName());
}