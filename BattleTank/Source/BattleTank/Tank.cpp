// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
// #include "TankAimingComponent.h"
// #include "TankBarrel.h"
// #include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));

}

float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);
	CurrentHealth -= DamageToApply;
	if (CurrentHealth <= 0)
	{
		OnDeath.Broadcast();
		if (DeathParticle->bHasBeenActivated == false)
		{
			DeathParticle->ActivateSystem();
			IdleSound->Stop();
		}
	}

	return DamageToApply;
}

float ATank::GetHealthPercentage() const
{
	return (float)CurrentHealth / (float)StartingHealth;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	// TankAimingComponent = FindComponentByClass<UTankAimingComponent>();

	CurrentHealth = StartingHealth;

	DeathParticle = Cast<UParticleSystemComponent>(GetDefaultSubobjectByName(TEXT("DeathParticle")));

	IdleSound = Cast<UAudioComponent>(GetDefaultSubobjectByName(TEXT("IdleSound")));
;}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
// void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);

// }

// void ATank::AimAt(FVector HitLocation) 
// {
// 	if (!TankAimingComponent) { return; }

// 	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
// }

// void ATank::SetBarrelReference(UTankBarrel* BarrelToSet) 
// {
// 	TankAimingComponent->SetBarrelReference(BarrelToSet);
// 	Barrel = BarrelToSet;
// }

// void ATank::SetTurretReference(UTankTurret* TurretToSet) 
// {
// 	TankAimingComponent->SetTurretReference(TurretToSet);
// }

// void ATank::Fire() 
// {
	// // UE_LOG(LogTemp, Warning, TEXT("Firing!"));

	// bool bIsReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeSeconds;
	// if (Barrel && bIsReloaded)
	// {
	// // Spawn projectile at a socket location on the barrel
	// auto Projectile = GetWorld()->SpawnActor<AProjectile>(
	// 		ProjectileBlueprint,
	// 		Barrel->GetSocketLocation(FName("Projectile")),
	// 		Barrel->GetSocketRotation(FName("Projectile"))
	// 	);
	// Projectile->LaunchProjectile(LaunchSpeed);
	// LastFireTime = FPlatformTime::Seconds();
	// }
// }

