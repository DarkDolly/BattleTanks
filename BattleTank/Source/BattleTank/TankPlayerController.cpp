// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "Tank.h"
#include "TankAimingComponent.h"
// #include "DrawDebugHelpers.h"

// ============================================================
class Animel
{
    public:
    virtual void NoiseMaker()
    {
        UE_LOG(LogTemp, Warning, TEXT("Animel noise!"));
    }
};

class Dog : public Animel
{
    public:
    void NoiseMaker() override
    {
        UE_LOG(LogTemp, Warning, TEXT("Woof!"));
    }
};

class Cat : public Animel
{
    public:
    void NoiseMaker() override
    {
        UE_LOG(LogTemp, Warning, TEXT("Meow!"));
    }
};
// ============================================================

void Stroke(Animel* animel)
{
    animel->NoiseMaker();
}
    void ATankPlayerController::BeginPlay() 
{
    Super::BeginPlay();
    auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if (AimingComponent)
    {
        FoundAimingComponent(AimingComponent);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AimingComponent not found by player controller!"))
    }
    
    // auto ControlledTank = GetControlledTank();

	// if (!ControlledTank)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("PlayerController is not possessing a tank"));
    // }
    // else 
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("PlayerController possessing %s"), *ControlledTank->GetName());
    // }

    // GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorRotation
    // (
    // FRotator(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation().Roll, GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation().Pitch, GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation().Yaw + 100.f)
    // ); // Fun mode (flipping tank)

// ============================================================
    Dog Dog;
    Cat Cat;
    Stroke(&Dog);
    Stroke(&Cat);
// ============================================================
}

void ATankPlayerController::SetPawn(APawn* InPawn)
{
    Super::SetPawn(InPawn);

    if (InPawn)
    {
        auto PossessedTank = Cast<ATank>(InPawn);
        if (!PossessedTank) { return; }

        // TODO Subscribe out local method to the tank's death event
        PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPlayerTankDeath);
    }
}

void ATankPlayerController::OnPlayerTankDeath()
{
    StartSpectatingOnly();
}

void ATankPlayerController::Tick(float DeltaTime) 
{
    AimTowardCrosshair();
//==================================================================================================================================================================================================
    // FVector PlayerViewLocation;

	// FRotator PlayerViewRotation;

	// GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);

	// // UE_LOG(LogTemp, Warning, TEXT("The player viewpoint location is %s and the player viewpoint rotation is %s"), *PlayerViewLocation.ToString(), *PlayerViewRotation.ToString());

	// FVector LineTraceEnd = FVector(PlayerViewLocation.X, PlayerViewLocation.Y, PlayerViewLocation.Z + 360.f) + PlayerViewRotation.Vector() * 2000.f;

	// DrawDebugLine(
	// 	GetWorld(),
	// 	PlayerViewLocation,
	// 	LineTraceEnd,
	// 	FColor(0, 255, 0),
	// 	false,
	// 	0.f,
	// 	0,
	// 	5
	// );
//==================================================================================================================================================================================================
}


// ATank* ATankPlayerController::GetControlledTank() const
// {
//     return Cast<ATank>(GetPawn();
// }

void ATankPlayerController::AimTowardCrosshair() 
{
    if (!GetPawn()) { return; }

    auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if (!AimingComponent) { return; }

    FVector HitLocation; // Out parameter
    if (GetSightRayHitLocation(HitLocation))
    {
        AimingComponent->AimAt(HitLocation);
    }
}

FVector ATankPlayerController::GetLineTraceEnd() const
{
    FVector PlayerViewLocation;

	FRotator PlayerViewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);

	FVector LineTraceEnd = FVector(PlayerViewLocation.X, PlayerViewLocation.Y, PlayerViewLocation.Z + 360.f) + PlayerViewRotation.Vector() * 2000.f;

    UE_LOG(LogTemp, Warning, TEXT("LineTraceEnd is at: %s"), *GetLineTraceEnd().ToString());

	return LineTraceEnd;
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
    // Find the crosshair position in pixel coordinates
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);
    auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairLocationX, ViewportSizeY * CrosshairLocationY);

    // "De-project" the screen position of the crosshair to a world location
    FVector LookDirection;
    if (GetLookDirection(ScreenLocation, LookDirection))
    {
        // UE_LOG(LogTemp, Warning, TEXT("ScreenLocation: %s"), *LookDirection.ToString());
    }

    // Line-trace along that LookDirection, and see what we hit (up to max range)
    return GetLookVectorHitLocation(LookDirection, HitLocation);

    return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
    FVector CameraWorldLocation;

    return DeprojectScreenPositionToWorld
    (
        ScreenLocation.X,
        ScreenLocation.Y,
        CameraWorldLocation,
        LookDirection
    );
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
    FHitResult HitResult;
    auto StartLocation = PlayerCameraManager->GetCameraLocation();
    auto EndLocation = StartLocation + (LookDirection * LineTraceRange);

    if (GetWorld()->LineTraceSingleByChannel
            (
            HitResult,
            StartLocation,
            EndLocation,
            ECC_Visibility
            )
        )
    {
        HitLocation = HitResult.Location;
        return true;
    }
    return false; // LineTrace didn't succeed
}