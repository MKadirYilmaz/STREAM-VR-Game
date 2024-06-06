// All rights belong to Kadir Yilmaz


#include "InteractionScripts/LeverSystem.h"


// Sets default values
ALeverSystem::ALeverSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	LeverBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverBaseMesh"));

	HandleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandleMesh"));

	RootComponent = CustomRoot;
	LeverBaseMesh->SetupAttachment(RootComponent);
	HandleMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ALeverSystem::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = HandleMesh->GetRelativeRotation();

	PositiveActivated = !StartActive;
	NegativeActivated = !StartActive;

	StartDeltaRotation = FMath::Clamp(StartDeltaRotation, -50.f, 50.f);
	HandleMesh->AddLocalRotation(FRotator(StartDeltaRotation, 0, 0));
	
}

// Called every frame
void ALeverSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HandComp)
	{
		CalculateLeverRotation(HandComp, HandleMesh);
	}
	if (ChildLever && IsHolding)
	{
		ChildLever->SetIsHolding(false);
		ChildLever->SetLeverPitchValue(CurrentLeverValue * 50);
	}

	//UE_LOG(LogTemp, Display, TEXT("Lever value: %f"), CurrentLeverValue);
	if (CurrentLeverValue >= LeverMaxValue)
	{
		if (!PositiveActivated) 
		{
			LeverActivated.Broadcast(true);
			PositiveActivated = true;
		}
	}
	else if (CurrentLeverValue <= LeverMinValue)
	{
		if (!NegativeActivated)
		{
			LeverActivated.Broadcast(false);
			NegativeActivated = true;
		}
	}
	else
	{
		PositiveActivated = false;
		NegativeActivated = false;
	}

}

void ALeverSystem::GrabLever(USceneComponent* GrabbedComp)
{
	if (Holdable)
	{
		HandComp = GrabbedComp;
		IsHolding = true;
	}
}

void ALeverSystem::ReleaseLever(USceneComponent* ReleasedComp)
{
	if (ReleasedComp == HandComp)
	{
		HandComp = nullptr;
		IsHolding = false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Released hand is wrong!"));
	}
}

