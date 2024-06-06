// Fill out your copyright notice in the Description page of Project Settings.


#include "RacingSystem.h"
#include "BaseTimer.h"
#include "Kismet/GameplayStatics.h"
#include "GameManagers/SaveScores.h"
#include "VehicleSystem/ToyPlane.h"
#include "PlaneRacingGame/RaceCheckpoint.h"

// Sets default values
ARacingSystem::ARacingSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomTimer = CreateDefaultSubobject<UBaseTimer>(TEXT("CustomTimer"));
}

// Called when the game starts or when spawned
void ARacingSystem::BeginPlay()
{
	Super::BeginPlay();
	if (PrevBestTiming.Num() != RaceCheckpoints.Num())
	{
		PrevBestTiming.SetNumZeroed(RaceCheckpoints.Num());
	}
}

// Called every frame
void ARacingSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RaceStarted && CurrentPlane)
	{
		UpdateRaceScreenUI(CurrentPlane->GetPlaneCurrentSpeed(), CustomTimer->GetCurrentTime());
	}
	CheckPlaneDistance();
}

void ARacingSystem::StartRace(int32 RouteIndex)
{
	if (!CustomTimer || RouteIndex >= RaceRouteCheckpoints.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("TimerHandler is nullptr or Race Route missing RacingSystem/StartRace"));
		return;
	}
	for (ARaceCheckpoint* _Checkpoint : RaceRouteCheckpoints[RouteIndex].RouteCheckpoints) // Butun checkpointleri tekrar aktif et
	{
		if (!_Checkpoint)
			continue;
		_Checkpoint->SetActiveCheckpoint(true);
	}
	CustomTimer->StartCounter();
	RaceScreenUI(true);

	RaceStarted = true;
	IsPlayerPlaying = true;
	CurrentCheckpoint = 0;
	TotalCurrentPenalty = 0;

	if (CurrentPlane)
	{
		//CurrentPlane->OnPlaneHit.AddDynamic(this, &ARacingSystem::CollisionRaceEnd);
		CurrentPlane->CollisionBroadcasted = false;
	}
}

void ARacingSystem::EndRace(EEndReason EndReason)
{
	RaceScreenUI(false);
	switch (EndReason)
	{
	case Finished:
		CurrentScore = CustomTimer->StopCounter() + TotalCurrentPenalty;
		//UE_LOG(LogTemp, Display, TEXT("Race finished: Score: %f "), CustomTimer->StopCounter() + TotalCurrentPenalty);
		if (CurrentScore < FastestTiming)
		{
			FastestTiming = CurrentScore;
			PrevBestTiming = CustomTimer->GetBreakingPoints();
		}
		RaceStarted = false;
		PrintBestScoreValues();
		AutoSaveScore(CurrentScore);
		PrintEndScreen(true);
		break;
	case Collision:
		RaceStarted = false;
		CustomTimer->StopCounter();
		PrintEndScreen(false);
		break;
	case Distance:
		RaceStarted = false;
		CustomTimer->StopCounter();
		PrintEndScreen(false);
		break;
		
	}
	if (CurrentPlane)
	{
		//CurrentPlane->OnPlaneHit.Clear();
		CurrentPlane->StopFlying();
	}
	RaceScreenUI(false);
	// Wait for a key press for restart or exit mini-game
}

void ARacingSystem::EndRacePlaneTooFar()
{
	EndRace(Distance);
}

void ARacingSystem::ExecuteRestart()
{
	CustomTimer->StartCountdown(1.5f, this);
	UE_LOG(LogTemp, Warning, TEXT("Restarting..."));
	CustomTimer->StopCountdown.AddDynamic(this, &ARacingSystem::RestartRace);
}

void ARacingSystem::AddCheckpointTime(int32 CheckpointIndex)
{
	if (!CustomTimer)
	{
		UE_LOG(LogTemp, Error, TEXT("CustomTimer is nullptr RacingSystem/AddCheckpointTime"));
		return;
	}
	if (CurrentCheckpoint == CheckpointIndex) // Checkpoint sirasi ile ilerlenip ilerlenmedigi kontrol edilir
	{
		if (CheckpointIndex + 1 == RaceRouteCheckpoints[CurrentRoute].RouteCheckpoints.Num()) // Egerki gecilen checkpoint son checkpoint ise yarisi bitir.
		{
			UE_LOG(LogTemp, Display, TEXT("Race Finished"));
			CustomTimer->AddBreakpoint(CheckpointIndex);
			EndRace(Finished);
			return;

		}
		UE_LOG(LogTemp, Display, TEXT("Entered from the right checkpoint. CurrentTime: %f"), CustomTimer->GetCurrentTime());
		CustomTimer->AddBreakpoint(CheckpointIndex);
		CurrentCheckpoint++;
		return;
	}
	else // Yanlis checkpoint siralamasi ile gecilmisse
	{
		for (int32 i = CurrentCheckpoint; i < CheckpointIndex; i++) // Arada kalan checkpointleri iptal et
		{
			if (RaceRouteCheckpoints[CurrentRoute].RouteCheckpoints.Num() > i)
			{
				RaceRouteCheckpoints[CurrentRoute].RouteCheckpoints[i]->SetActiveCheckpoint(false);
			}
		}
		UE_LOG(LogTemp, Error, TEXT("Wrong order checkpoint. Current = %d, Checkpoint = %d"), CurrentCheckpoint, CheckpointIndex);
		AddPenalty(CalculatePenaltyTime(CurrentCheckpoint, CheckpointIndex));
		if (CheckpointIndex + 1 == RaceRouteCheckpoints[CurrentRoute].RouteCheckpoints.Num())
		{
			UE_LOG(LogTemp, Display, TEXT("Race Finished"));
			CustomTimer->AddBreakpoint(CheckpointIndex);
			EndRace(Finished);
			return;
		}
		CurrentCheckpoint = CheckpointIndex + 1; // En son gecilen checkpointe dogru siralamayi kaydir
		return;
	}
	
}

float ARacingSystem::CalculatePenaltyTime(int32 CurrentCheckpointIndex, int32 PassedCheckpointIndex)
{
	if (RaceRouteCheckpoints[CurrentRoute].RouteCheckpoints.Num() > PassedCheckpointIndex && CurrentPlane)
	{
		float _PassedDistance = (RaceRouteCheckpoints[CurrentRoute].RouteCheckpoints[CurrentCheckpointIndex]->GetActorLocation() - 
			RaceRouteCheckpoints[CurrentRoute].RouteCheckpoints[PassedCheckpointIndex]->GetActorLocation()).Length();
		return (_PassedDistance / CurrentPlane->GetPlaneMaxSpeed()) + 1;
	}
	else
	{
		return 0.f;
	}
}

void ARacingSystem::AddPenalty(float PenaltyTime)
{
	TotalCurrentPenalty += PenaltyTime;
	UE_LOG(LogTemp, Display, TEXT("Added penalty amount: %f seconds."), PenaltyTime);
}

void ARacingSystem::PrintBestScoreValues()
{
	for (int i = 0; i < PrevBestTiming.Num(); i++)
	{
		UE_LOG(LogTemp, Display, TEXT("%d Checkpoint: %f seconds"), i, PrevBestTiming[i]);
	}
	UE_LOG(LogTemp, Display, TEXT("Fastest score: %f"), FastestTiming);
}

void ARacingSystem::RestartRace(AActor* Self)
{
	if (Self == this && CurrentPlane)
	{
		AutoQuitInterrupted = true;
		CurrentPlane->ReturnToStartingPoint();
		StartRace(CurrentRoute);
	}
}

void ARacingSystem::CollisionRaceEnd()
{
	if (RaceStarted) // Yaris halihazirda hala devam ediyorsa
	{
		EndRace(Collision);
	}
}

void ARacingSystem::QuitRace()
{
	if (CurrentPlane && !AutoQuitInterrupted)
	{
		CurrentPlane->StopFlying();
		CurrentPlane->ReturnToStartingPoint();
	}
}

void ARacingSystem::CheckPlaneDistance()
{
	if (RaceRouteCheckpoints[CurrentRoute].RouteCheckpoints.Num() < 1 || !RaceStarted || !IsPlayerPlaying)
		return;
	//int32 _MinDistanceIndex = 0;
	//float _MinDistance = FVector::Distance(CurrentPlane->GetActorLocation(), RaceRouteCheckpoints[CurrentRoute].RouteCheckpoints[0]->GetActorLocation());
	for (int32 i = 0; i < RaceRouteCheckpoints[CurrentRoute].RouteCheckpoints.Num(); i++)
	{
		if (MaxDepatureDistance > FVector::Distance(RaceRouteCheckpoints[CurrentRoute].RouteCheckpoints[i]->GetActorLocation(), 
			CurrentPlane->GetActorLocation())) // Ucak herhangi bir checkpointe max dist'den yakinsa
		{
			GetWorldTimerManager().ClearTimer(PlaneDistanceCounter);
			WarnPlayerReturnZone(false);
			DistanceWarningMade = false;
			return;
		}
	}
	if (!DistanceWarningMade)
	{
		WarnPlayerReturnZone(true);
		GetWorldTimerManager().SetTimer(PlaneDistanceCounter, this, &ARacingSystem::EndRacePlaneTooFar, 5.f, false);
		DistanceWarningMade = true;
	}
	// Warn player then end race
}

float ARacingSystem::GetCurrentTime() const
{
	return CustomTimer->GetCurrentTime();
}

