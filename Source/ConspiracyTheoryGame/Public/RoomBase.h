// Team Development of a Conspiracy Theory Game for GameBOX.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomBase.generated.h"

UENUM(BlueprintType)
enum class ERoomDirection : uint8
{
    North,
    East,
    South,
    West
};

USTRUCT(BlueprintType)
struct FRoomExit
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ERoomDirection Direction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator Rotation;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool bIsConnected = false;
};

UCLASS()
class CONSPIRACYTHEORYGAME_API ARoomBase : public AActor
{
    GENERATED_BODY()

public:
    ARoomBase();

    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    FVector GetLocalExitLocation(ERoomDirection Direction) const;

    void SpawnPatrolTargetPoint();
    FVector GetExitDirectionWorld(const FRoomExit& Exit) const { return GetActorRotation().RotateVector(Exit.Rotation.Vector()); }

    void InitializeRoomExits(float DistanceFromCenter = 500.f);
    FVector GetExitLocation(ERoomDirection Direction) const;
    FRotator GetExitRotation(ERoomDirection Direction) const;

    UFUNCTION(BlueprintCallable)
    FTransform GetWorldExitTransform(const FRoomExit& Exit) const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* RoomMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Exits")
    bool bIsStartRoom = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Exits")
    bool bIsBossRoom = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Exits")
    TArray<FRoomExit> RoomExits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Exits")
    float ExitOffset = 3000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Exits")
    bool bUseNorthExit = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Exits")
    bool bUseSouthExit = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Exits")
    bool bUseEastExit = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Exits")
    bool bUseWestExit = true;

private:
    bool bExitsInitialized = false;
};
