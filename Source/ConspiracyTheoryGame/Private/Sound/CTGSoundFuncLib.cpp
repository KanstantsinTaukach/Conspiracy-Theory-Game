// Team Development of a Conspiracy Theory Game for GameBOX.

#include "Sound/CTGSoundFuncLib.h"
#include "Sound/SoundClass.h"

void UCTGSoundFuncLib::SetSoundClassVolume(USoundClass* SoundClass, float InVolume)
{
    if (!SoundClass) return;

    SoundClass->Properties.Volume = FMath::Clamp(InVolume, 0.0f, 1.0f);
}

float UCTGSoundFuncLib::GetSoundClassVolume(USoundClass* SoundClass)
{
    if (!SoundClass) return 0.0f;

    return SoundClass->Properties.Volume;
}
