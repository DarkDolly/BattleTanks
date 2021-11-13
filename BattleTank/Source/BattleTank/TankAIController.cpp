// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "TankPlayerController.h"
#include "Tank.h" // So we can inpmliment on death
// Depends on movement component via pathfinding system

void ATankAIController::BeginPlay() 
{
    Super::BeginPlay();

}

void ATankAIController::SetPawn(APawn* InPawn)
{
    Super::SetPawn(InPawn);

    if (InPawn)
    {
        auto PossessedTank = Cast<ATank>(InPawn);
        if (!PossessedTank) { return; }

        // TODO Subscribe out local method to the tank's death event
        PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossedTankDeath);
    }
}

void ATankAIController::OnPossedTankDeath()
{
    if (!GetPawn()) { return; }
    GetPawn()->DetachFromControllerPendingDestroy();
}

void ATankAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    auto ControlledTank = GetPawn();
    auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

    if (!PlayerTank && !ControlledTank) { return; }
    {
        //move towards player
        MoveToActor(PlayerTank, AcceptanceRadius); // TODO check radius is in cm

        auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
        if (!AimingComponent) { return; }
        //DONE aim towards player
        /*AimingComponent->AimAt(PlayerTank->FindComponentByClass<USceneComponent>()->GetComponentLocation());*/ // OLD METHOD
        USceneComponent* AimPoint = Cast<USceneComponent>(PlayerTank->GetDefaultSubobjectByName(TEXT("AimPoint"))); // NEW METHOD (More accurate by using a component called "AimPoint"
        if (!AimPoint) { return; }
        AimingComponent->AimAt(AimPoint->GetComponentLocation());

        //DONE Only fire if locked
        float Range = 100.f;
        if (AimingComponent->GetFiringState() == EFiringState::Locked)
        {
                AimingComponent->Fire(); // DONE Limit fire rate
        }
     }
}

