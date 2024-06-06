// Fill out your copyright notice in the Description page of Project Settings.


#include "MathMiniGame/MathLaserMarker.h"
#include "MathMiniGame/MathBomb.h"
#include "MathMiniGame/MathGameSystem.h"
#include "MathMiniGame/MathLaserShooter.h"

AMathLaserMarker::AMathLaserMarker()
{

	PrimaryActorTick.bCanEverTick = true;

	LaserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserMesh"));
	LaserEndMarker = CreateDefaultSubobject<USceneComponent>(TEXT("LaserEndMarker"));
	LaserMesh->SetupAttachment(RootComponent);
	LaserEndMarker->SetupAttachment(RootComponent);
}

void AMathLaserMarker::BeginPlay()
{
	Super::BeginPlay();

	InitialLaserScale = FVector::Distance(LaserMesh->GetComponentLocation(), LaserEndMarker->GetComponentLocation());
}

void AMathLaserMarker::LockOnBomb()
{
	if (ScannedBomb && WorldMathSystem)
	{
		UE_LOG(LogTemp, Display, TEXT("You are now locked on to a bomb"));
		WorldMathSystem->SetupScreenAndButtons(ScannedBomb->GetMainExpression(), ScannedBomb->GetChoice1(), ScannedBomb->GetChoice2(), ScannedBomb->GetAnswerIndex(), ScannedBomb);
		AMathLaserShooter* _LaserShooter = WorldMathSystem->GetTiedLaserShooter();
		if (_LaserShooter)
		{
			_LaserShooter->SetTargetBomb(ScannedBomb);
			_LaserShooter->SetShouldTrace(true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There is no bomb found to lock on to it"));
	}
}

void AMathLaserMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector _Start = Mesh->GetComponentLocation();
	FVector _End = _Start + Mesh->GetRightVector() * LaserMaxDistance;
	
	bool _Hitted = GetWorld()->LineTraceSingleByChannel(MathHitResult, _Start, _End, ECC_Visibility);
	if (_Hitted)
	{
		ScannedBomb = Cast<AMathBomb>(MathHitResult.GetActor());
		
		LaserMesh->SetRelativeScale3D(FVector(0.05f, 0.05f, MathHitResult.Distance / InitialLaserScale));
		//DrawDebugLine(GetWorld(), _Start, MathHitResult.ImpactPoint, FColor::Blue);
		//UE_LOG(LogTemp, Display, TEXT("Laser Hitted %s"), *MathHitResult.GetActor()->GetName());
	}
	else
	{
		LaserMesh->SetRelativeScale3D(FVector(0.05f, 0.05f, LaserMaxDistance / InitialLaserScale));
		//DrawDebugLine(GetWorld(), _Start, _End, FColor::Blue);
		//UE_LOG(LogTemp, Display, TEXT("Laser Not Hitted"));
	}
}

