// Copyright 2017 Fe-eze Anya

#include "Grabber.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for Duty!"));
	
	/// Look for attached Physics Handle
	PhysicsHandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandler) {
		// Physics handle is found
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No Physics Handler found in %s"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player viewpoint this tick
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	// Log out to test
	/*UE_LOG(LogTemp, Warning, TEXT("Position %s, Rotation %s"), *PlayerViewpointLocation.ToString(), *PlayerViewpointRotation.ToString());*/

	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector()*Reach;

	// Draw red line to visualize
	DrawDebugLine(GetWorld(), PlayerViewpointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 20.f);

	// Set query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	
	/// Raycast to reach distance
	FHitResult Hit;
	bool IsHittingSomething = GetWorld()->LineTraceSingleByObjectType(
		OUT Hit, 
		PlayerViewpointLocation, 
		LineTraceEnd, 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), 
		TraceParameters
	);
	
	// See what we hit
	if (IsHittingSomething) {
		UE_LOG(LogTemp, Warning, TEXT("Can grab %s"), *(Hit.GetActor()->GetName()));
	}
}

