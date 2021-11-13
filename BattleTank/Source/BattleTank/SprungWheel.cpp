// Fill out your copyright notice in the Description page of Project Settings.


#include "SprungWheel.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;


	Spring = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Spring"));
	SetRootComponent(Spring);
	Spring->SetLinearVelocityDrive(false, false, true);
	Spring->SetLinearDriveParams(1000.f, 200.f, 0.f);
	Spring->SetLinearZLimit(ELinearConstraintMotion::LCM_Free, 100.f);
	Spring->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	Spring->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	Spring->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);
	Spring->SetLinearPositionDrive(false, false, true);


	//SpringTopSphere = CreateDefaultSubobject<USphereComponent>(FName("Spring Top Sphere"));
	///*SpringTopSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);*/ SpringTopSphere->SetupAttachment(RootComponent); // Another quicker way of attaching
	//SpringTopSphere->SetSimulatePhysics(true);

	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	Wheel->SetupAttachment(RootComponent);

	SpringButtomSphere = CreateDefaultSubobject<USphereComponent>(FName("Spring Buttom Sphere"));
	/*SpringButtomSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);*/ SpringButtomSphere->SetupAttachment(Wheel); // Another quicker way of attaching
	SpringButtomSphere->SetSimulatePhysics(true);

	Axle = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Axle"));
	Axle->SetupAttachment(Wheel);
	Axle->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	Axle->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.f);
	Axle->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();

	//if (GetAttachParentActor())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Not null, attached to %s"), *GetAttachParentActor()->GetName())
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Null"))
	//}

	Wheel ->SetNotifyRigidBodyCollision(true);
	Wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);

	SetupConstraint();

	DrivingSound = Cast<UAudioComponent>(GetDefaultSubobjectByName(TEXT("DrivingSound")));
}

void ASprungWheel::SetupConstraint()
{
	if (!GetAttachParentActor()) { return; }
	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());
	if (!BodyRoot) { return; }
	Spring->SetConstrainedComponents(BodyRoot, NAME_None, SpringButtomSphere, NAME_None);
	Axle->SetConstrainedComponents(SpringButtomSphere, NAME_None, Wheel, NAME_None);
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		TotalForceMagnitudeThisFrame = 0.f;
	}
	/*UE_LOG(LogTemp, Warning, TEXT("Tick %f"), GetWorld()->GetTimeSeconds())*/
	//if (!(DrivingSound->IsPlaying()))
	//{
	//	DrivingSound->Play();
	//}
}

void ASprungWheel::AddDrivingForce(float ForceMagnitude)
{
	TotalForceMagnitudeThisFrame += ForceMagnitude;
}

void ASprungWheel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	/*UE_LOG(LogTemp, Warning, TEXT("Getting hit %f"), GetWorld()->GetTimeSeconds())*/
	ApplyForce();
}

void ASprungWheel::ApplyForce()
{
	Wheel->AddForce(SpringButtomSphere->GetForwardVector() * TotalForceMagnitudeThisFrame);
}