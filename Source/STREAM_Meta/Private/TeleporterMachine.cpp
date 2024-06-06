// All rights belong to Kadir Yilmaz


#include "TeleporterMachine.h"
#include "InteractionScripts/LeverSystem.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATeleporterMachine::ATeleporterMachine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MachineMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MachineMesh"));
	TeleportArea = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TeleportArea"));

	RootComponent = MachineMesh;
	TeleportArea->SetupAttachment(RootComponent);

	TeleportArea->SetCollisionProfileName(FName("OverlapAll"));
	
}

// Called when the game starts or when spawned
void ATeleporterMachine::BeginPlay()
{
	Super::BeginPlay();
	
	if (TiedLever)
	{
		TiedLever->LeverActivated.AddDynamic(this, &ATeleporterMachine::OnLeverActivated);
	}
}

// Called every frame
void ATeleporterMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATeleporterMachine::ReverseDoors()
{
	ExecDoorAnimation(false);
}

void ATeleporterMachine::ExecuteTeleport()
{
	TArray<AActor*> OverlappedActors;
	TeleportArea->GetOverlappingActors(OverlappedActors);
	FVector _TeleportLocation;
	FRotator _TeleportRotation;
	if (TeleportFlipFlop)
	{
		_TeleportLocation = TeleportLocation1;
		_TeleportRotation = TeleportRotation1;
		TeleportFlipFlop = false;
	}
	else
	{
		_TeleportLocation = TeleportLocation2;
		_TeleportRotation = TeleportRotation2;
		TeleportFlipFlop = true;
	}
	FVector _Displacement = _TeleportLocation - GetActorLocation();
	SetActorLocation(_TeleportLocation);
	SetActorRotation(_TeleportRotation);

	for (AActor* _Actor : OverlappedActors)
	{
		if (_Actor != this)
		{
			_Actor->SetActorLocation(_Actor->GetActorLocation() + _Displacement + FVector(0,0,5.f));
		}
	}
}

void ATeleporterMachine::OnLeverActivated(bool IsPositive)
{
	ExecDoorAnimation(true);
	// Teleport
}

