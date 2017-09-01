// Copyright 2017 Fe-eze Anya

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
//#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWBUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// How far ahead can we reach in cm
	float Reach = 100.f;
		
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Find (assumed) attached Physics handle Component
	void FindPhysicsHandleComponent();

	// Setup (assumed) attached Input Component
	void SetupInputComponent();

	// Grab what's in reach of raycast bolt
	void Grab();
	
	// Release an object that's been grabbed
	void Release();

	// Return the first physics body in reach (quick raycast to relieve hot loop)
	const FHitResult GetFirstPhysicsBodyInReach();
};
