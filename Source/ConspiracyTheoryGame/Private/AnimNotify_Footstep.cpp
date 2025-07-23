// Team Development of a Conspiracy Theory Game for GameBOX.


#include "AnimNotify_Footstep.h"
#include "EnemyCharacter.h"
#include "CTGCharacter.h"

void UAnimNotify_Footstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();

    if (ACTGCharacter* Player = Cast<ACTGCharacter>(Owner))
    {
        Player->PlayFootstep();
    }
    else if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Owner))
    {
        Enemy->PlayFootstep(Enemy->bIsRunning);  // ”бедись, что у врага есть логика, определ€юща€ бежит он или нет
    }
}