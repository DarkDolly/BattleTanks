// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"
#include "SprungWheel.h"
#include "SpawnPoint.h"

UTankTrack::UTankTrack()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
    Super::BeginPlay();

    /*OnComponentHit.AddDynamic(this, &UTankTrack::OnHit); // Removed after adding wheels */
}

//void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) // Removed after adding wheels
//{
//    // Drive the track
//    DriveTrack();
//
//    // Apply sideways force
//    ApplySidewaysForce();
//
//    /*CurrentThrottle = 0.f; // Removed after adding wheels */
//} // Removed after adding wheels

//void UTankTrack::ApplySidewaysForce() // Removed after adding wheels
//{
//    // Calculate the slippage speed
//    // ----------------------------------------------------------------------------
//    //auto TankSideVector = GetOwner()->GetActorRightVector().GetSafeNormal();
//    //auto TankVelocity = GetOwner()->GetVelocity();
//    // ----------------------------------------------------------------------------
//    auto SlippageSpeed = FVector::DotProduct(/*TankSideVector, TankVelocity*/ GetRightVector(), GetComponentVelocity());
//    // Work out required acceleration this frame to currect
//    auto CurrectionAcceleration = -SlippageSpeed / GetWorld()->GetDeltaSeconds() * GetRightVector();
//    // Calculate and apply sideways force for (F = M * A)
//    auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
//    auto CurrectionForce = TankRoot->GetMass() * CurrectionAcceleration / 4; // Devided by 2 because there are 2 tracks (Decided to devide by 4 for less force allowing slight side movement)
//    TankRoot->AddForce(CurrectionForce);
//} // Removed after adding wheels

void UTankTrack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTankTrack::SetThrottle(float Throttle) 
{
    // auto Time = GetWorld()->GetTimeSeconds();
    // auto Name = GetName();
	// UE_LOG(LogTemp, Warning, TEXT("Throttle: %f"), Throttle);

	// TODO clamp actual throttle value so player can't over-drive (Solved in TankMovementComponent.cpp)

    /*CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1.f, 1.f); // Changed after adding wheels */ auto CurrentThrottle = FMath::Clamp<float>(Throttle, -1.f, 1.f);
    DriveTrack(CurrentThrottle);
}

void UTankTrack::/*DriveTrack() // Changed after adding wheels */DriveTrack(float CurrentThrottle)
{
    auto ForceApplied = /* GetForwardVector() * // Removed after adding wheels */ CurrentThrottle * TrackMaxDrivingForce;
    /* auto ForceLocation = GetComponentLocation();
    auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    TankRoot->AddForceAtLocation(ForceApplied, ForceLocation); // Removed after adding wheels */

    auto Wheels = GetWheels();
    auto ForcePerWheel = ForceApplied / Wheels.Num();
    for (ASprungWheel* Wheel : Wheels)
    {
        Wheel->AddDrivingForce(ForcePerWheel);
    }
}

TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
    TArray <ASprungWheel*> ResultArray;
    TArray <USceneComponent*> Children;
    GetChildrenComponents(true, Children);
    for (USceneComponent* Child : Children)
    {
        auto SpawnPointChild = Cast<USpawnPoint>(Child);
        if (!SpawnPointChild) { continue; }

        AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();
        auto SprungWheel = Cast<ASprungWheel>(SpawnedChild);
        if (!SprungWheel) { continue; }

        ResultArray.Add(SprungWheel);
    }

    return ResultArray;
}