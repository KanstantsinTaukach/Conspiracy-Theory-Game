// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Player/CTGKsilanCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/Gameplaystatics.h"
#include "Sound/SoundCue.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ACTGKsilanCharacter::ACTGKsilanCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    FollowSocketName = "TopSocket";
}

void ACTGKsilanCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!OwnerActor)
    {
        return;
    }

    const auto Character = Cast<ACharacter>(OwnerActor);
    if (Character)
    {
        FVector TargetLocation = Character->GetMesh()->GetSocketLocation(FollowSocketName);

        FVector CurrentLocation = GetActorLocation();

        FVector MoveDirection = TargetLocation - CurrentLocation;
        float SpeedMultiplier = 2.0f;

        GetCharacterMovement()->MoveSmooth(MoveDirection * SpeedMultiplier, DeltaTime);
        GetCharacterMovement()->MoveSmooth(TargetLocation - CurrentLocation, DeltaTime);

        FRotator TargetRotation = OwnerActor->GetActorRotation() - FRotator(0.0f, 90.0f, 0.0f);
        SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 20.0f));

        float Distance = MoveDirection.Size();
        if (Distance > TeleportDistance)
        {
            if (TeleportSound)
            {
                UGameplayStatics::PlaySound2D(GetWorld(), TeleportSound);
            }

            if (TeleportEffect)
            {
                UNiagaraComponent* NS = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TeleportEffect, TargetLocation);
            }

            SetActorLocation(TargetLocation);

            return;
        }
    }
}
