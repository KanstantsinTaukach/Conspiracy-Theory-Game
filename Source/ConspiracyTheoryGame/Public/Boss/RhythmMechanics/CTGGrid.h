// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTGCoreTypes.h"
#include "CTGGrid.generated.h"

class UStaticMeshComponent;

UCLASS()
class CONSPIRACYTHEORYGAME_API ACTGGrid : public AActor
{
    GENERATED_BODY()

public:
    ACTGGrid();

    virtual void Tick(float DeltaTime) override;

    void SetModel(const FSettings& InSettings, uint32 InCellSize);

    void UpdateColors(const FGridColors& Colors);

protected:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Origin;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* GridMesh;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UMaterialInstanceDynamic* GridMaterial;

    FDim GridDim;
    uint32 CellSize;
    uint32 WorldWidth;
    uint32 WorldHeight;

    void DrawGrid();
};
