// Team Development of a Conspiracy Theory Game for GameBOX.

#include "UI/CTGGameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

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
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}
