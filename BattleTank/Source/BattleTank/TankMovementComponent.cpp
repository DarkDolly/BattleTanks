// Fill out your copyright notice in the Description page of Project Settings.


#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialise(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet) 
{
    LeftTrack = LeftTrackToSet;
    RightTrack = RightTrackToSet;
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    // No need to call Super because we replace the functionallity

    auto TankForwardDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();
    auto AIForwardIntention = MoveVelocity.GetSafeNormal();

    auto ForwardThrow = FVector::DotProduct(TankForwardDirection, AIForwardIntention);
    IntendMoveForward(ForwardThrow);

    auto TurnThrow = FVector::CrossProduct(TankForwardDirection, AIForwardIntention).Z;
    IntendTurn(TurnThrow);

    //    UE_LOG(LogTemp, Warning, TEXT("MoveVelocity: %s"),  *AIForwardIntention.ToString())
}

void UTankMovementComponent::IntendMoveForward(float Throw) 
{
	// UE_LOG(LogTemp, Warning, TEXT("Intend move forward throw: %f"), Throw);
    if(!LeftTrack || !RightTrack) { return; }
    LeftTrack->SetThrottle(Throw);
    RightTrack->SetThrottle(Throw);
    // TODO Prevent double speed with double input
}

void UTankMovementComponent::IntendTurn(float TurnThrow) 
{
    // UE_LOG(LogTemp, Warning, TEXT("Intend turn throw: %f"), TurnThrow);

    if(!LeftTrack || !RightTrack) { return; }
    LeftTrack->SetThrottle(TurnThrow);
    RightTrack->SetThrottle(-TurnThrow);
    // TODO Prevent double speed with double input
}

