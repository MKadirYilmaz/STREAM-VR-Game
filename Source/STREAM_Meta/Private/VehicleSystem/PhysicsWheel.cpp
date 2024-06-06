// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleSystem/PhysicsWheel.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "VehicleSystem/BaseCar.h"
#include "VehicleSystem/CarTireComponent.h"

APhysicsWheel::APhysicsWheel()
{
	WheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh"));
	WheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("WheelConstraint"));
	WheelMesh->SetupAttachment(RootComponent);
	WheelConstraint->SetupAttachment(RootComponent);
	GetBodyMesh()->SetSimulatePhysics(false);
	WheelMesh->SetSimulatePhysics(true);

	WheelConstraint->ComponentName1.ComponentName = FName("Mesh");
	WheelConstraint->ComponentName2.ComponentName = FName("WheelMesh");

	WheelConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	WheelConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	WheelConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Limited, 90.f);

	WheelMesh->SetAngularDamping(1.3f);

	//GetGrabbingPointMesh()->SetupAttachment(WheelMesh, FName("WheelGrabbingPoint"));
}

void APhysicsWheel::BeginPlay()
{
	Super::BeginPlay();
	StartLocalRotation = WheelMesh->GetRelativeRotation();
	
}

void APhysicsWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TotalRotationChange = GetWheelAngularVelocity();

	//UE_LOG(LogTemp, Display, TEXT("Manual: %f"), TotalRotationChange);
	if (TiedBaseCar)
	{
		FVector _Start = TiedBaseCar->GetActorLocation() + FVector(0, 0, 100.f);
		FVector _NewDirection = TiedBaseCar->GetActorForwardVector().RotateAngleAxis((TotalRotationChange / 3), TiedBaseCar->GetActorUpVector());

		TiedBaseCar->GetFrontRightTire()->SetCurrentTireDirection(_NewDirection);
		TiedBaseCar->GetFrontLeftTire()->SetCurrentTireDirection(_NewDirection);
		
	}
	
}

float APhysicsWheel::GetWheelAngularVelocity()
{
	FRotator _RotationChange = (WheelMesh->GetRelativeRotation() - StartLocalRotation).GetNormalized();
	if (!TiedBaseCar)
		return 0.f;
	if (_RotationChange.Pitch < 0.f)
	{
		TiedBaseCar->SetRightLeftValue(-1);
	}
	else
	{
		TiedBaseCar->SetRightLeftValue(1);
	}
	return _RotationChange.Pitch;
}
