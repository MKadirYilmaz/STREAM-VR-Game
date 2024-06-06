// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleSystem/PlaneControlStick.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
APlaneControlStick::APlaneControlStick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	ControlStickMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ControlStickMesh"));
	ConnectionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConnectionMesh"));
	StickConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("StickConstraint"));

	ControlStickMesh->SetSimulatePhysics(true);
	ControlStickMesh->SetEnableGravity(false);

	RootComponent = DefaultRoot;
	ControlStickMesh->SetupAttachment(RootComponent);
	ConnectionMesh->SetupAttachment(RootComponent);
	StickConstraint->SetupAttachment(RootComponent);

	StickConstraint->ComponentName1.ComponentName = FName("ConnectionMesh");
	StickConstraint->ComponentName2.ComponentName = FName("ControlStickMesh");
}

// Called when the game starts or when spawned
void APlaneControlStick::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocalRotation = StickConstraint->GetRelativeRotation();
}

// Called every frame
void APlaneControlStick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float _PitchDelta = GetPitchAngularDelta();
	float _RollDelta = GetRollAngularDelta();

	UE_LOG(LogTemp, Display, TEXT("PitchDelta: %f, YawDelta: %f"), _PitchDelta, _RollDelta);


}

float APlaneControlStick::GetPitchAngularDelta()
{
	FRotator _DeltaRotation = (ControlStickMesh->GetRelativeRotation() - StartLocalRotation).GetNormalized();

	return _DeltaRotation.Pitch;
}

float APlaneControlStick::GetRollAngularDelta()
{
	FRotator _DeltaRotation = (ControlStickMesh->GetRelativeRotation() - StartLocalRotation).GetNormalized();

	return _DeltaRotation.Roll;
}

