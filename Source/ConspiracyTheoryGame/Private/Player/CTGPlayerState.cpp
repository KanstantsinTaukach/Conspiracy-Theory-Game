// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Player/CTGPlayerState.h"

void ACTGPlayerState::AddPoints(int32 Delta) 
{
    if (!ensure(Delta > 0)) return;

    Points += Delta;

    OnPointsChanged.Broadcast(this, Points, Delta);
}

bool ACTGPlayerState::RemovePoints(int32 Delta)
{
    if (!ensure(Delta > 0)) return false;

    Points -= Delta;
    if (Points < 0)
    {
        Points = 0;
    }

    OnPointsChanged.Broadcast(this, Points, Delta);

    return true;
}