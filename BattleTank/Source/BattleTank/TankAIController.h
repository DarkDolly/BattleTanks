// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

private:
	void BeginPlay() override;

	// How close can the AI tank get to the player
	float AcceptanceRadius = 8000.f;

	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION()
	void OnPossedTankDeath();

public:
	virtual void Tick(float DeltaTime) override;

protected:
	
};
