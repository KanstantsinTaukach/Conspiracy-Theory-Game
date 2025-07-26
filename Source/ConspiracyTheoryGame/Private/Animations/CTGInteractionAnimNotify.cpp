// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Animations/CTGInteractionAnimNotify.h"

void UCTGInteractionAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}