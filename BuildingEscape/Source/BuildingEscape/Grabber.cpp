// Fill out your copyright notice in the Description page of Project Settings.

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

	FindPhysicsHandleComponent();

	SetupInputComponent();
}

///Look for attached Input Component
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s found InputComponent"), *GetOwner()->GetName());
		///Bind input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing InputComponent"), *GetOwner()->GetName())
	}
}

///Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		// Physics handle found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing PhysicsHandleComponent"), *GetOwner()->GetName())
			//PhysicsHandle = GetOwner()->AddComponent()
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();

	auto ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
		UE_LOG(LogTemp, Warning, TEXT("%s grabbed"), *ActorHit->GetName());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector PlayerVPPos;
	FRotator PlayerVPRot;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerVPPos, OUT PlayerVPRot);

	FVector LineTraceEnd = PlayerVPPos + PlayerVPRot.Vector() * Reach;
	//DrawDebugLine(GetWorld(), PlayerVPPos, LineTraceEnd, FColor::Red, true, 1.f, 0.f, 10.f);

	///Line-trace (AKA ray-cast) out to reach distance
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerVPPos,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;
}

void UGrabber::Release()
{
	if (PhysicsHandle->GrabbedComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grab Released"));
		PhysicsHandle->ReleaseComponent();
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		FVector PlayerVPPos;
		FRotator PlayerVPRot;

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerVPPos, OUT PlayerVPRot);

		FVector LineTraceEnd = PlayerVPPos + PlayerVPRot.Vector() * Reach;

		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

