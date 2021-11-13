// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTurret.h"


void UTankTurret::RotateTurret(float RelativeSpeed) 
{
    RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.f, 1.f);
    auto RotationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
    auto Rotation = RotationChange + GetRelativeRotation().Yaw;

    SetRelativeRotation(FRotator(0.f, Rotation, 0.f));
}
