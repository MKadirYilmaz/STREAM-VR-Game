// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RacingSystem.generated.h"


enum EEndReason
{
	Finished,
	Collision,
	Distance
};

USTRUCT(BlueprintType)
struct FRaceRouteCheckpoints
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TArray<class ARaceCheckpoint*> RouteCheckpoints;

};
UCLASS()
class STREAM_META_API ARacingSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARacingSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void AutoSaveScore(float Score);
	UFUNCTION(BlueprintImplementableEvent)
	void PrintEndScreen(bool RaceCompleted);
	UFUNCTION(BlueprintImplementableEvent)
	void RaceScreenUI(bool bShow);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRaceScreenUI(float PlaneSpeed, float Time);
	UFUNCTION(BlueprintImplementableEvent)
	void WarnPlayerReturnZone(bool ShouldWarn);

	UFUNCTION(BlueprintCallable)
	void StartRace(int32 RouteIndex);
	void EndRace(EEndReason EndReason);
	void EndRacePlaneTooFar();

	UFUNCTION(BlueprintCallable)
	void ExecuteRestart();

	void AddCheckpointTime(int32 CheckpointIndex);
	float CalculatePenaltyTime(int32 CurrentCheckpointIndex, int32 PassedCheckpointIndex);
	void AddPenalty(float PenaltyTime);

	void PrintBestScoreValues();
	UFUNCTION()
	void CollisionRaceEnd();
protected:
	UFUNCTION()
	void RestartRace(AActor* Self);
	UFUNCTION()
	void QuitRace();

	void CheckPlaneDistance();

	UPROPERTY(EditAnywhere)
	TArray<class ARaceCheckpoint*> RaceCheckpoints;

	UPROPERTY(EditAnywhere)
	TArray<FRaceRouteCheckpoints> RaceRouteCheckpoints;

	class USaveScores* SavedScores;
	class UBaseTimer* CustomTimer;
	UPROPERTY(EditAnywhere)
	class AToyPlane* CurrentPlane;
	UPROPERTY(EditAnywhere)
	float MaxDepatureDistance = 1000.f;

	bool IsPlayerPlaying = false;
	bool RaceStarted = false;
	bool AutoQuitInterrupted = false;
	bool DistanceWarningMade = false;

	int32 CurrentCheckpoint = 0;
	int32 CurrentRoute = 0;

	float CurrentScore = 0;

	float FastestTiming = 1000000.f; // En hizli yapilan yarisin toplam suresi (Baslagic icin asiri yuksek bir deger girildi)
	UPROPERTY(EditAnywhere)
		TArray<float> PrevBestTiming; // En hizli yapilan yarisin checkpoint basina gecen sureleri

	float TotalCurrentPenalty = 0;
	FTimerHandle PlaneDistanceCounter;

public:
	TArray<ARaceCheckpoint*> GetRacingCheckpoints(int32 RouteIndex) { return RaceRouteCheckpoints[RouteIndex].RouteCheckpoints; }
	int32 GetRouteAmount() { return RaceRouteCheckpoints.Num(); }
	
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentRoute() const { return CurrentRoute; }
	void SetCurrentRoute(int32 NewRoute) { CurrentRoute = NewRoute; }

	UFUNCTION(BlueprintCallable)
	bool GetIsPlayerPlaying() const { return IsPlayerPlaying; }
	UFUNCTION(BlueprintCallable)
	void SetIsPlayerPlaying(bool IsPlaying) { IsPlayerPlaying = IsPlaying; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentScore() const { return CurrentScore; }

	UFUNCTION(BlueprintCallable)
	float GetCurrentTime() const;

	void SetCurrentPlane(AToyPlane* NewCurrentPlane) { CurrentPlane = NewCurrentPlane; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AToyPlane* GetCurrentPlane() const { return CurrentPlane; }
};
