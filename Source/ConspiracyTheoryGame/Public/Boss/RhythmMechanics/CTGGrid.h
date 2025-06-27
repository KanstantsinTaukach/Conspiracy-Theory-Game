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
    ACTGGrid();

    virtual void Tick(float DeltaTime) override;

    void SetModel(const FSettings& InSettings, int32 InCellSize);

protected:
    virtual void BeginPlay() override;

private:
    FDim GridDim;
    int32 CellSize;
    int32 WorldWidht;
    int32 WorldHeight;

    void DrawGrid();
};
