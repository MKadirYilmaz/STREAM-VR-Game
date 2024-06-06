// Fill out your copyright notice in the Description page of Project Settings.


#include "MathMiniGame/MathBomb.h"
#include "MathMiniGame/MathGameSystem.h"

// Sets default values
AMathBomb::AMathBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));

	RootComponent = BombMesh;

}

// Called when the game starts or when spawned
void AMathBomb::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("Bomb's duration: %f"), Duration);

}

// Called every frame
void AMathBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldOffset(FVector(0, 0, -FallingSpeed * DeltaTime), true, &SurfaceDetector);
}

void AMathBomb::SetDuration(float MaxDuration)
{
	Duration = MaxDuration; 
	FallingSpeed = (GetActorLocation().Z - SurfaceZ) / Duration;
	GetWorldTimerManager().SetTimer(ExplosionHandler, this, &AMathBomb::DestroyTheBomb, Duration, false);
}

void AMathBomb::EliminateBomb()
{
	ExplosionSystem();
}

void AMathBomb::DestroyTheBomb()
{
	if (MathGameManager)
	{
		//MathGameManager->DeleteBombFromArray(this);
		MathGameManager->BombDurationRunOut(this);
	}
	ExplosionSystem();
}

void AMathBomb::DestroyTheBombPunishment()
{
	if (MathGameManager)
	{
		//MathGameManager->DeleteBombFromArray(this);
	}
	ExplosionSystem();
}

void AMathBomb::FastDropBomb()
{
	FallingSpeed = (GetActorLocation().Z - SurfaceZ);
	
	if (ExplosionHandler.IsValid())
	{
		//GetWorldTimerManager().ClearTimer(ExplosionHandler);

		GetWorldTimerManager().SetTimer(ExplosionHandler, this, &AMathBomb::DestroyTheBombPunishment, 1.f);
	}
}

void AMathBomb::GameFinishedDestroy()
{
	Destroy();
}

void AMathBomb::SetAddDynamic()
{
	if (MathGameManager)
	{
		MathGameManager->GameFinished.AddDynamic(this, &AMathBomb::GameFinishedDestroy);
	}
}

