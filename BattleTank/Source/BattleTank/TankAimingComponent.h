// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

// Enum for aiming state
UENUM()
enum class EFiringState : uint8 { Reloading, Aiming, Locked, OutOfAmmo };
// TODO add OutOfRange state, in this state (assign new color) you and the AI tanks can't fire *Optional: the turret and barrel don't move OR the AI turret and barrel don't move* and if possible the AI don't move towards you.

// Forward declarations
class UTankBarrel;
class UTankTurret;
class AProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	// void SetBarrelReference(UTankBarrel* BarrelToSet);

	// void SetTurretReference(UTankTurret* TurretToSet);

	// void AimAt(FVector HitLocation, float LaunchSpeed);

	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void InitialiseAiming(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet, UStaticMeshComponent* HullToSet);

	EFiringState GetFiringState() const;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	int32 GetRoundsLeft() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringState FiringState = EFiringState::Reloading;

private:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsBarrelMoving();

	FVector AimDirection;

	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;
	UStaticMeshComponent* Hull = nullptr;

	UPROPERTY(EditAnywhere, Category = "Firing")
	float LaunchSpeed = 8000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditAnywhere, Category = "Firing")
	float ReloadTimeSeconds = 7.f;

	void MoveBarrelTowards(FVector AimDirec);

	double LastFireTime = 0;

	UPROPERTY(EditAnywhere, Category = "Firing")
	int32 RoundsLeft = 30;

};
