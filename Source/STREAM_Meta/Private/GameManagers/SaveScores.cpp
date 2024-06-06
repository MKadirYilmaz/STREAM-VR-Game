// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagers/SaveScores.h"

USaveScores::USaveScores()
{
	FlightGameScores.SetNum(5); // Toplamda 5 tane yol olacagi varsayildi.
}

void USaveScores::NewFlightGameScore(int32 NewScore, FName Name, int32 RouteIndex)
{
	if (FlightGameScores.Num() <= RouteIndex)
	{
		UE_LOG(LogTemp, Error, TEXT("NewFlightGameScore RouteIndex is out of bounds"));
		for (int i = FlightGameScores.Num(); i <= RouteIndex; i++)
		{
			TArray<LeaderboardBlock> _NewRoute;
			FlightGameScores.Add(_NewRoute);
		}
	}
	FlightGameScores[RouteIndex].Add(LeaderboardBlock(NewScore, Name));
	/*for (int i = FlightGameScores[RouteIndex].Num() - 1; i > 0; i--)
	{
		if (FlightGameScores[RouteIndex][i].Score < FlightGameScores[RouteIndex][i - 1].Score)
		{
			LeaderboardBlock _Temp = FlightGameScores[RouteIndex][i - 1];
			FlightGameScores[RouteIndex][i - 1] = FlightGameScores[RouteIndex][i];
			FlightGameScores[RouteIndex][i] = _Temp;
		}
	}*/
}

void USaveScores::NewMathGameScore(int32 NewScore, FName Name)
{
	MathGameScores.Add(LeaderboardBlock(NewScore, Name));

	for (int i = MathGameScores.Num() - 1; i > 0; i--)
	{
		if (MathGameScores[i].Score > MathGameScores[i - 1].Score)
		{
			LeaderboardBlock _Temp = MathGameScores[i - 1];
			MathGameScores[i - 1] = MathGameScores[i];
			MathGameScores[i] = _Temp;
		}
	}
}

void USaveScores::PrintLeaderboard(const TArray<TArray<LeaderboardBlock>>& Leaderboard, int32 LeaderboardParam)
{
	if (LeaderboardParam >= Leaderboard.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Array out of bounds USaveScores/PrintLeaderboard"));
		return;
	}
	for (int32 i = 0; i < Leaderboard[LeaderboardParam].Num(); i++)
	{
		UE_LOG(LogTemp, Display, TEXT("Placement %d, Name: %s, Score: %f"), i + 1, *Leaderboard[LeaderboardParam][i].PlayerName.ToString(), Leaderboard[LeaderboardParam][i].Score);
	}
}

void USaveScores::PrintLeaderboard(const TArray<LeaderboardBlock>& Leaderboard)
{
	int32 _Placement = 1;
	for (LeaderboardBlock _Block : Leaderboard)
	{
		UE_LOG(LogTemp, Display, TEXT("Placement %d, Name: %s, Score: %f"), _Placement, *_Block.PlayerName.ToString(), _Block.Score);
		_Placement++;
	}
}

void USaveScores::SetFlightRouteAmount(int32 RouteAmount)
{
	for (int i = 0; i < RouteAmount; i++)
	{
		if (FlightGameScores.Num() <= i)
		{
			TArray<LeaderboardBlock> _NewRoute;
			FlightGameScores.Add(_NewRoute);
		}
	}
}

void USaveScores::ClearFlightLeaderboard()
{
	FlightGameScores.Empty();
	SetFlightRouteAmount(5);
}

