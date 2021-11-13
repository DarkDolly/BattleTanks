// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// -1 is max downward speed and +1 is max upward speed
	void ElevateBarrel(float RelativeSpeed);

protected:

private:
	UPROPERTY(EditAnywhere, Category = Setup)
	float MaxDegreesPerSecond = 6.f;

	UPROPERTY(EditAnywhere, Category = Setup)
	float MaxElevationDegrees = 26.f;

	UPROPERTY(EditAnywhere, Category = Setup)
	float MinElevationDegrees = -2.f;

};
