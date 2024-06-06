// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneRacingGame/RaceCheckpoint.h"
#include "RacingSystem.h"

// Sets default values
ARaceCheckpoint::ARaceCheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CheckPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CheckPointMesh"));
	MiddleZone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MiddleZone"));

	RootComponent = CheckPointMesh;
	MiddleZone->SetupAttachment(RootComponent);

	MiddleZone->SetCollisionProfileName(FName("OverlapAll"));
}

// Called when the game starts or when spawned
void ARaceCheckpoint::BeginPlay()
{
	Super::BeginPlay();
	SetActiveCheckpoint(false);
	MiddleZone->OnComponentBeginOverlap.AddDynamic(this, &ARaceCheckpoint::OnOverlapBegin);
	
}

void ARaceCheckpoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (RacingSystem && IsCheckpointActive)
	{
		RacingSystem->AddCheckpointTime(CheckpointIndex);
		SetActiveCheckpoint(false);
	}
}

// Called every frame
void ARaceCheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARaceCheckpoint::SetActiveCheckpoint(bool bActive)
{
	CheckPointMesh->SetVisibility(bActive);
	IsCheckpointActive = bActive;
}



