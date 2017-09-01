// Copyright 2017 Fe-eze Anya

#include "Grabber.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
	
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
		// Physics handle is found
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No Physics Handler found in %s"), *GetOwner()->GetName());
	}
}

/// Look for input componenet(which appears at runtime only) and log error if not found
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input component found"));
		/// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No Input component found in %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
	/// Raycast and see if we hit any actors with physics body collision set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();

	/// If we hit one, attach the physics handle
	if (HitResult.GetActor() != nullptr) {
		// Attach physics handle
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released?"));
	// Release Physics Handle
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player viewpoint this tick
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector()*Reach;

	// If we're grabbing something (ie if the physics handle is attached)
	if (PhysicsHandle->GrabbedComponent) {
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	// otherwise do nothing
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Get player viewpoint this tick
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector()*Reach;

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
	return Hit;
}
