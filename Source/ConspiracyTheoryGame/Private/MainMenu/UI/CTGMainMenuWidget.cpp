// Team Development of a Conspiracy Theory Game for GameBOX.

#include "MainMenu/UI/CTGMainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "CTGGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogCTGMainMenuWidget, All, All)


void UCTGMainMenuWidget::NativeOnInitialized() 
{
    Super::NativeOnInitialized();

    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UCTGMainMenuWidget::OnStartGame);
    }
}

void UCTGMainMenuWidget::OnStartGame() 
{
    if (!GetWorld()) return;

    const auto CTGGameInstanse = GetWorld()->GetGameInstance<UCTGGameInstance>();
    if (!CTGGameInstanse) return;

    if (CTGGameInstanse->GetStartupLevelName().IsNone())
    {
        UE_LOG(LogCTGMainMenuWidget, Error, TEXT("Level name is NONE"));
        return;
    }

    UGameplayStatics::OpenLevel(this, CTGGameInstanse->GetStartupLevelName());
}