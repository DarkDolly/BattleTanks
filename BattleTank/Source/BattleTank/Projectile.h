// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UParticleSystemComponent;
class URadialForceComponent;
class UAudioComponent;

UCLASS()
class BATTLETANK_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LaunchProjectile(float Speed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void OnTimerExpire();

	UProjectileMovementComponent* ProjectileMovement = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Damage")
	float ProjectileDamage = 20.f;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	UStaticMeshComponent* CollisionMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Particles")
	UParticleSystemComponent* LaunchBlast = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Particles")
	UParticleSystemComponent* ImpactBlast = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Particles")
	UParticleSystemComponent* LaunchTrail = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Forces")
	URadialForceComponent* ExplosionForce = nullptr;

	UAudioComponent* HittingSound = nullptr;

	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	float TimeBeforeProjectileDestroy = 5.f;
};
