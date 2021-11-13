// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TankAimingComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement Component"));
	ProjectileMovement->bAutoActivate = false;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SetRootComponent(CollisionMesh);
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	//CollisionMesh->SetVisibility(false); Set this false if you don't want a projectile mesh and only a collision mesh

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	LaunchTrail = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Trail"));
	LaunchTrail ->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &AProjectile::OnHit);

	HittingSound = Cast<UAudioComponent>(GetDefaultSubobjectByName(TEXT("HitSound")));

}

void AProjectile::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->FindComponentByClass<UStaticMeshComponent>() || OtherActor->FindComponentByClass<USphereComponent>())
	{
	ImpactBlast->Activate();
	HittingSound->Play();
	}
	/*CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionMesh->SetVisibility(false);*/ // Old method to make sure the projectile doesn't keep hitting stuff and also to make it invisible
	LaunchTrail->Deactivate();
	LaunchTrail->SetVisibility(false);
	// ExplosionForce->FireImpulse(); // Activate if I want the tanks to move from the explosion (In the future use the same method with a barrel attached with spring and fire the impulse on spawn and not one hit to make the barrel go back from the "recoil"
	SetRootComponent(LaunchTrail);
	CollisionMesh->DestroyComponent();

	UGameplayStatics::ApplyRadialDamage
	(
		this,
		ProjectileDamage,
		GetActorLocation(),
		ExplosionForce->Radius,
		UDamageType::StaticClass(),
		TArray<AActor*>()
	);

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AProjectile::OnTimerExpire, TimeBeforeProjectileDestroy);
	
}

void AProjectile::OnTimerExpire()
{
	this->Destroy();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::LaunchProjectile(float Speed) 
{
	auto Time = GetWorld()->GetTimeSeconds();
	// UE_LOG(LogTemp, Warning, TEXT("%f Projectile fires at the speed of %f"), Time, Speed);
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovement->Activate();
}
