// Team Development of a Conspiracy Theory Game for GameBOX.

#include "UI/CTGPauseWidget.h"
#include "Components/Button.h"
#include "Gameframework/GameModeBase.h"

void UCTGPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &UCTGPauseWidget::OnClearPause);
    }
}

void UCTGPauseWidget::OnClearPause()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->ClearPause();
}