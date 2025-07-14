// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Player/CTGPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "CTGSaveGame.h"

void ACTGPlayerState::BeginPlay()
{
    Super::BeginPlay();

    if (UGameplayStatics::DoesSaveGameExist(TEXT("CTGSlot"), 0))
    {
        UCTGSaveGame* SaveGame = Cast<UCTGSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("CTGSlot"), 0));
        if (SaveGame)
        {
            PreviousPoints = Points;
            Points = SaveGame->SavedPoints;

            OnPointsChanged.Broadcast(this, Points, 0);
        }
    }
    LoadProgress();


}

void ACTGPlayerState::AddPoints(int32 Delta) 
{
    if (!ensure(Delta > 0)) return;

    PreviousPoints = Points;
    Points += Delta;

    OnPointsChanged.Broadcast(this, Points, Delta);
}

bool ACTGPlayerState::RemovePoints(int32 Delta)
{
    if (!ensure(Delta > 0)) return false;

    PreviousPoints = Points;
    Points -= Delta;
    if (Points < 0)
    {
        Points = 0;
    }

    OnPointsChanged.Broadcast(this, Points, Delta);

    return true;
}

void ACTGPlayerState::SavePoints()
{
    UCTGSaveGame* SaveGameInstance = Cast<UCTGSaveGame>(UGameplayStatics::CreateSaveGameObject(UCTGSaveGame::StaticClass()));

    if (SaveGameInstance)
    {
        SaveGameInstance->SavedPoints = Points;
        UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("CTGSlot"), 0);
    }
}

void ACTGPlayerState::LoadPoints()
{
    if (UGameplayStatics::DoesSaveGameExist(TEXT("CTGSlot"), 0))
    {
        UCTGSaveGame* LoadedGame = Cast<UCTGSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("CTGSlot"), 0));

        if (LoadedGame)
        {
            Points = LoadedGame->SavedPoints;
        }
    }
}
void ACTGPlayerState::LoadProgress()
{
    if (UGameplayStatics::DoesSaveGameExist("CTGSlot", 0))
    {
        UCTGSaveGame* LoadedGame = Cast<UCTGSaveGame>(UGameplayStatics::LoadGameFromSlot("CTGSlot", 0));
        if (LoadedGame)
        {
            SetPoints(LoadedGame->SavedPoints);
            UE_LOG(LogTemp, Warning, TEXT("Loaded points: %d"), LoadedGame->SavedPoints);


            OnPointsChanged.Broadcast(this, Points, 0);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No save game exists."));
    }
}