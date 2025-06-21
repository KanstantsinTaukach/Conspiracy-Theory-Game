// Team Development of a Conspiracy Theory Game for GameBOX.

#include "MainMenu/UI/CTGMainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

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
    const FName StartupLevelName = "TestLevelTony";
    UGameplayStatics::OpenLevel(this, StartupLevelName);
}