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

	// ...
	//UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"))
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	FVector PlayerVPPos;
	FRotator PlayerVPRot;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerVPPos, OUT PlayerVPRot);

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Position: %s"), *PlayerVPPos.ToString(), *PlayerVPRot.ToString())

	FVector LineTraceEnd = PlayerVPPos + PlayerVPRot.Vector() * Reach;
	DrawDebugLine(GetWorld(), PlayerVPPos, LineTraceEnd, FColor::Red, true, 1.f, 0.f, 10.f);
}

