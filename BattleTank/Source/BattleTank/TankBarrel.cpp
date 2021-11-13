// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrel.h"


void UTankBarrel::ElevateBarrel(float RelativeSpeed) 
{
    // Move the barrel the right amount this frame

	// Given a max elevation speed, and the frame time
    RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.f, 1.f);
    auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
    auto RawNewElevation = ElevationChange + GetRelativeRotation().Pitch;

    auto NewElevation = FMath::Clamp(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);
    SetRelativeRotation(FRotator(NewElevation, 0.f, 0.f));
}
