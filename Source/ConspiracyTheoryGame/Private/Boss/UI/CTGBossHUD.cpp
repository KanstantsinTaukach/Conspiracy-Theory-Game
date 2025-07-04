// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Boss/UI/CTGBossHUD.h"
#include "Boss/UI/CTGBossGameplayWidget.h"

void ACTGBossHUD::BeginPlay() 
{
    Super::BeginPlay();

    BossGameplayWidget = CreateWidget<UCTGBossGameplayWidget>(GetWorld(), BossGameplayWidgetClass);
    check(BossGameplayWidget);
    BossGameplayWidget->AddToViewport();
}