// All rights belong to Kadir Yilmaz


#include "InteractionScripts/PhysicsDoorSystem.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
APhysicsDoorSystem::APhysicsDoorSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorHandle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorHandle"));
	DoorCloseZone = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorCloseZone"));
	DoorAndHandleTarget = CreateDefaultSubobject<USceneComponent>(TEXT("DoorAndHandleTarget"));

	FrameToDoorConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FrameToDoorConstraint"));
	DoorToHandleConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("DoorToHandleConstraint"));

	RootComponent = DoorFrame;

	DoorAndHandleTarget->SetupAttachment(RootComponent);
	DoorMesh->SetupAttachment(RootComponent);
	DoorCloseZone->SetupAttachment(RootComponent);
	DoorHandle->SetupAttachment(DoorMesh);
	FrameToDoorConstraint->SetupAttachment(RootComponent);
	DoorToHandleConstraint->SetupAttachment(RootComponent);

	DoorFrame->SetSimulatePhysics(false);
	DoorMesh->SetSimulatePhysics(true);
	DoorHandle->SetSimulatePhysics(false);

	FrameToDoorConstraint->SetConstrainedComponents(DoorFrame, NAME_None, DoorMesh, NAME_None);
	DoorToHandleConstraint->SetConstrainedComponents(DoorMesh, NAME_None, DoorHandle, NAME_None);

}

// Called when the game starts or when spawned
void APhysicsDoorSystem::BeginPlay()
{
	Super::BeginPlay();
	
	DoorCloseRotation = DoorMesh->GetComponentRotation();
	/*
	if(!OverrideStartCondition)
		SetDoorOpenCondition(IsOpen);
	*/
}

// Called every frame
void APhysicsDoorSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GrabberComp)
	{
		DoorAndHandleTarget->SetWorldLocation(GrabberComp->GetComponentLocation());
		CalculateHandleRotation(DoorAndHandleTarget, DoorHandle);
		
		if (IsLocked) // Kilitliyse devam etme
		{
			DoorMesh->SetSimulatePhysics(false);
			return;
		}
		
		if (IsDoorBeingPulled || IsDoorOpenPosition) // Kol cekiliyor veya kapi acik konumunda
		{
			//DoorMesh->SetSimulatePhysics(true);
			CalculateDoorRotationForHand(DoorAndHandleTarget, DoorMesh);
		}
		else // Kapi cekilmiyor ve kapi kapali durumda
		{
			//DoorMesh->SetRelativeRotation(FRotator(0, -90.f, 0));
			ReturnDoorToOriginalPositon();
			//DoorMesh->SetSimulatePhysics(false);
		}
	}
}

void APhysicsDoorSystem::SetDoorOpenCondition(bool bIsOpen)
{
	if (bIsOpen)
	{
		DoorMesh->SetSimulatePhysics(true);
		FrameToDoorConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 45.f);
		IsOpen = true;
		ShouldTraceHand = true;
		UE_LOG(LogTemp, Display, TEXT("Door is now open"));
	}
	else
	{
		DoorMesh->SetSimulatePhysics(false);
		//DoorMesh->SetWorldRotation(DoorCloseRotation);
		FrameToDoorConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
		IsOpen = false;
		ShouldTraceHand = false;
		UE_LOG(LogTemp, Display, TEXT("Door is now closed"));
	}
}

void APhysicsDoorSystem::SetDoorLockedCondition(bool bIsLocked)
{
	
}

void APhysicsDoorSystem::GrabHandle(USceneComponent* GrabComp)
{
	if (GrabComp)
	{
		GrabberComp = GrabComp;
		IsGrabbing = true;
	}
}

void APhysicsDoorSystem::ReleaseHandle(USceneComponent* ReleaseComp)
{
	if (GrabberComp == ReleaseComp)
	{
		GrabberComp = nullptr;
		IsGrabbing = false;
	}
}

float APhysicsDoorSystem::GetRotationDifference() const
{
	FVector _Direction = (DoorMesh->GetComponentLocation() - DoorAndHandleTarget->GetComponentLocation()).GetSafeNormal();
	float _DotResult = FVector::DotProduct(_Direction, DoorMesh->GetRightVector());
	//UE_LOG(LogTemp, Display, TEXT("Dot: %f"), _DotResult);
	if (_DotResult > 0.f) // Behind the door
	{
		return 15.f;
	}
	else // Front the door
	{
		return -15.f;
	}
}

