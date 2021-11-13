// Fill out your copyright notice in the Description page of Project Settings.
// Adding comment to test sourcetree

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

// class UTankBarrel;
// class UTankAimingComponent;
// class AProjectile;
class UParticleSystemComponent;
class UAudioComponent;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// void AimAt(FVector HitLocation);

	// UFUNCTION(BlueprintCallable, Category = Setup)
	// void SetBarrelReference(UTankBarrel* BarrelToSet);
	
	// UFUNCTION(BlueprintCallable, Category = Setup)
	// void SetTurretReference(UTankTurret* TurretToSet);

	// UFUNCTION(BlueprintCallable, Category = Input)
	// void Fire();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercentage() const;

	FTankDelegate OnDeath;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// UPROPERTY(BlueprintReadOnly)
	// UTankAimingComponent* TankAimingComponent = nullptr;

private:
	// Sets default values for this pawn's properties
	ATank();

	// UPROPERTY(EditAnywhere, Category = "Setup")
	// TSubclassOf<AProjectile> ProjectileBlueprint;

	// Remove once fire is moved to AimingComponent
	// UPROPERTY(EditAnywhere, Category = "Firing")
	// float LaunchSpeed = 8000.f;

	// UPROPERTY(EditAnywhere, Category = "Firing")
	// float ReloadTimeSeconds = 7.f;

	// Local Barrel reference for spawning the projectile
	// UTankBarrel* Barrel = nullptr;

	// double LastFireTime = 0;

	UPROPERTY(EditAnywhere, Category = "Health")
	int32 StartingHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 CurrentHealth;

	UParticleSystemComponent* DeathParticle = nullptr;

	UAudioComponent* IdleSound = nullptr;
};
