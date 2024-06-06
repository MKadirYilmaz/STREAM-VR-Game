// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveScores.generated.h"

/**
 * 
 */
struct LeaderboardBlock
{
	LeaderboardBlock() {}
	LeaderboardBlock(int32 CScore, FName CPlayerName)
	{
		Score = CScore;
		CPlayerName = PlayerName;
	}
	float Score;
	FName PlayerName;
};
UCLASS()
class STREAM_META_API USaveScores : public USaveGame
{
	GENERATED_BODY()

public:
	USaveScores();

	void NewFlightGameScore(int32 NewScore, FName Name, int32 RouteIndex);

	void NewMathGameScore(int32 NewScore, FName Name);

private:
	TArray<TArray<LeaderboardBlock>> FlightGameScores;
	TArray<LeaderboardBlock> MathGameScores;

public:
	void PrintLeaderboard(const TArray<TArray<LeaderboardBlock>>& Leaderboard, int32 LeaderboardParam);
	void PrintLeaderboard(const TArray<LeaderboardBlock>& Leaderboard);

	TArray<LeaderboardBlock> GetFlightLeaderboard(int32 LeaderboardParam) { return FlightGameScores[LeaderboardParam]; }

	void SetFlightRouteAmount(int32 RouteAmount);
	void ClearFlightLeaderboard();
	
};
