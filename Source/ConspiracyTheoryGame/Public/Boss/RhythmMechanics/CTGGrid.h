// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTGCoreTypes.h"
#include "CTGGrid.generated.h"

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGGrid : public AActor
{
    GENERATED_BODY()

public:
    ACTGGrid(const FDim& Dim);

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;
};
