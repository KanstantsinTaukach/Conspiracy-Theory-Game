// Team Development of a Conspiracy Theory Game for GameBOX.

#include "UI/CTGPauseWidget.h"
#include "Components/Button.h"
#include "Gameframework/GameModeBase.h"

bool UCTGPauseWidget::Initialize()
{
    const auto InitStatus = Super::Initialize();

    if (ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &UCTGPauseWidget::OnClearPause);
    }

    return InitStatus;
}

void UCTGPauseWidget::OnClearPause()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->ClearPause();
}