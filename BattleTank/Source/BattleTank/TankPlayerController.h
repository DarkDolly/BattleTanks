// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

// class ATank;
class UTankAimingComponent;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
    float CrosshairLocationX = 0.5f;

	UPROPERTY(EditAnywhere)
    float CrosshairLocationY = 0.33333;

	UPROPERTY(EditAnywhere)
    float LineTraceRange = 1000000.f;

	void AimTowardCrosshair();

	bool GetSightRayHitLocation(FVector& HitLocation) const;
	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;

	FVector GetLineTraceEnd() const;

	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION()
	void OnPlayerTankDeath();

public:
	void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
protected:
	// UFUNCTION(BlueprintCallable, Category = "Setup")
	// ATank* GetControlledTank() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimingComponentReference);

};
