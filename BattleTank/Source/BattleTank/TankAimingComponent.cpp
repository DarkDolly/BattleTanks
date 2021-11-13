// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "TankTurret.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
// {
// 	Barrel = BarrelToSet;
// }

// void UTankAimingComponent::SetTurretReference(UTankTurret* TurretToSet)
// {
// 	Turret = TurretToSet;
// }

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	LastFireTime = /*FPlatformTime::Seconds()*/ GetWorld()->GetTimeSeconds();
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (RoundsLeft <= 0)
	{
		FiringState = EFiringState::OutOfAmmo;
	}
	else if (bool bIsReloaded = (/*FPlatformTime::Seconds()*/ GetWorld()->GetTimeSeconds() - LastFireTime) < ReloadTimeSeconds)
	{
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

int32 UTankAimingComponent::GetRoundsLeft() const
{
	return RoundsLeft;
}

void UTankAimingComponent::AimAt(FVector HitLocation /*, float LaunchSpeed*/) 
{
	// auto TankName = GetOwner()->GetName();
	// auto BarrelLocation = Barrel->GetComponentLocation();
	// UE_LOG(LogTemp, Warning, TEXT("%s is aiming at %s from %s"), *TankName, *HitLocation.ToString(), *BarrelLocation.ToString());

	if (!Barrel) {return;}
	
	FVector LaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	// Calculate the LaunchVelocity
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
			(
				this,
				LaunchVelocity,
				StartLocation,
				HitLocation,
				LaunchSpeed,
				false,
				0.f,
				0.f,
				ESuggestProjVelocityTraceOption::DoNotTrace
			);
	if (bHaveAimSolution)
	{
		// FString TankName = GetOwner()->GetName();
		AimDirection = LaunchVelocity.GetSafeNormal();
		// UE_LOG(LogTemp, Warning, TEXT("%s is aiming at %s"), *TankName, *AimDirection.ToString());
		MoveBarrelTowards(AimDirection);
		// auto Time = GetWorld()->GetTimeSeconds();
		// UE_LOG(LogTemp, Warning, TEXT("%f aim solve found"), Time);
	}
	else
	{
		// auto Time = GetWorld()->GetTimeSeconds();
		// UE_LOG(LogTemp, Warning, TEXT("%f No aim solve found"), Time);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirec) 
{
	if (!Barrel || !Turret) { return; }

	// Work-out difference between current barrel roation, and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirec.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	// UE_LOG(LogTemp, Warning, TEXT("AimRotator is %s"), *DeltaRotator.ToString())

	Barrel->ElevateBarrel(DeltaRotator.Pitch); // TODO Remove magic number
	
	// Always Yaw the shortest way
	if (FMath::Abs(DeltaRotator.Yaw) < 180.f)
	{
		Turret->RotateTurret(DeltaRotator.Yaw);
	}
	else
	{
		Turret->RotateTurret(-DeltaRotator.Yaw);
	}
}

void UTankAimingComponent::InitialiseAiming(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet, UStaticMeshComponent* HullToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
	Hull = HullToSet;
}

void UTankAimingComponent::Fire()
{
	// UE_LOG(LogTemp, Warning, TEXT("Firing!"));

	/*bool bIsReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeSeconds;*/
	if (/*bIsReloaded*/ FiringState != EFiringState::Reloading && FiringState != EFiringState::OutOfAmmo)
	{
	if (!Barrel) { return; }
	if (!ProjectileBlueprint) { return; }
	// Spawn projectile at a socket location on the barrel
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
		);
	Projectile->LaunchProjectile(LaunchSpeed);
	LastFireTime = /*FPlatformTime::Seconds()*/ GetWorld()->GetTimeSeconds();
	RoundsLeft--;
	}
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!Barrel) { return false; }

	auto BarrelDirection = Barrel->GetForwardVector();

	//if (!BarrelDirection.Equals(AimDirection, 0.01f)) { return true; } // Vectors are equal
	//		
	//return false;

	return !BarrelDirection.Equals(AimDirection, 0.01f);
}