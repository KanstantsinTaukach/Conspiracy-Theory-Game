// Team Development of a Conspiracy Theory Game for GameBOX.

#include "MainMenu/UI/CTGMainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "CTGGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogCTGMainMenuWidget, All, All)

void UCTGMainMenuWidget::NativeOnInitialized() 
{
    Super::NativeOnInitialized();

    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UCTGMainMenuWidget::OnStartGame);
    }

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &UCTGMainMenuWidget::OnQuitGame);
    }
}

void UCTGMainMenuWidget::OnStartGame() 
{
    if (!GetWorld()) return;

    const auto CTGGameInstance = GetWorld()->GetGameInstance<UCTGGameInstance>();
    if (!CTGGameInstance) return;

    if (CTGGameInstance->GetStartupLevelName().IsNone())
    {
        UE_LOG(LogCTGMainMenuWidget, Error, TEXT("Level name is NONE"));
        return;
    }

    UGameplayStatics::OpenLevel(this, CTGGameInstance->GetStartupLevelName());
}

void UCTGMainMenuWidget::OnQuitGame() 
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}