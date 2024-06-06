// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EmptyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("EmptyRoot"));
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorHingeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorHingeMesh"));
	DoorHinge = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("DoorHinge"));

	RootComponent = EmptyRoot;
	DoorMesh->SetupAttachment(RootComponent);
	DoorHingeMesh->SetupAttachment(RootComponent);
	DoorHinge->SetupAttachment(RootComponent);

	DoorMesh->SetSimulatePhysics(true);
	DoorMesh->SetEnableGravity(false);

	DoorHinge->ComponentName1.ComponentName = FName("DoorHingeMesh");
	DoorHinge->ComponentName2.ComponentName = FName("DoorMesh");

	DoorHinge->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 90.f);
	DoorHinge->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	DoorHinge->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);
}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

