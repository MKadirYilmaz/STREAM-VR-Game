// Fill out your copyright notice in the Description page of Project Settings.


#include "LifterSystem.h"
#include "PhysicalButton.h"

// Sets default values
ALifterSystem::ALifterSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LifterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LifterMesh"));

	RootComponent = LifterMesh;

}

// Called when the game starts or when spawned
void ALifterSystem::BeginPlay()
{
	Super::BeginPlay();
	for (APhysicalButton* _Caller : LifterCallers)
	{
		if(_Caller)
			_Caller->ButtonPressed.AddDynamic(this, &ALifterSystem::CallLifter);
	}
	for (APhysicalButton* _FloorButton : TiedButtons)
	{
		if (_FloorButton)
		{
			_FloorButton->ButtonPressed.AddDynamic(this, &ALifterSystem::ButtonSystem);
		}
	}
}

// Called every frame
void ALifterSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ShouldMove)
	{
		LifterMesh->SetWorldLocation(LifterMesh->GetComponentLocation() + LifterMesh->GetUpVector() * LifterDirection * DeltaTime);
		if (FVector::Distance(LifterMesh->GetComponentLocation(), TargetLocation) < 5.f) // Asansor kata cok yaklasmissa direkt o noktaya isinla
		{
			LifterMesh->SetWorldLocation(TargetLocation);
			CurrentFloor = TargetFloor;
			ShouldMove = false;
			LifterStoppingAnim();
		}
	}
}

void ALifterSystem::MoveLifter()
{
	if (FloorStopZAxis.Num() <= TargetFloor)
	{
		UE_LOG(LogTemp, Error, TEXT("TargetFloor is beyond reach"));
	}
	else if (TargetFloor != CurrentFloor)
	{
		TargetLocation = FVector(LifterMesh->GetComponentLocation().X, LifterMesh->GetComponentLocation().Y, FloorStopZAxis[TargetFloor]);
		if (CurrentFloor > TargetFloor)
		{
			LifterDirection = -LifterSpeed;
		}
		else
		{
			LifterDirection = LifterSpeed;
		}
		LifterMovingAnim();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetFloor and CurrentFloor are the same"));
	}
}

void ALifterSystem::CallLifter(int32 CalledFloor)
{
	if (CurrentFloor != CalledFloor)
	{
		TargetFloor = CalledFloor;
		MoveLifter();
		UE_LOG(LogTemp, Display, TEXT("Elevator called. Coming to %d floor"), CalledFloor);
	}
}

void ALifterSystem::ButtonSystem(int32 FloorIndex)
{
	//AddToQueue(FloorIndex);
	HighlightPressedButton(FloorIndex);
	TargetFloor = FloorIndex;
	MoveLifter();
	UE_LOG(LogTemp, Display, TEXT("Elevetor triggered with button"));
}

void ALifterSystem::AddToQueue(int32 NewFloorIndex) // Unfinished
{
	TargetFloors.Add(NewFloorIndex); // En sona ekle
	for (int32 i = TargetFloors.Num() - 2; i >= 0; i--) // Tek tek basa dogru al  1 - 3 - 4 - 2 => 1 - 2 - 3 - 4
	{
		if (TargetFloors[i] > NewFloorIndex) 
		{
			int32 _Temp = TargetFloors[i];
			TargetFloors[i] = NewFloorIndex;
			TargetFloors[i + 1] = _Temp;
		}
	}
}

