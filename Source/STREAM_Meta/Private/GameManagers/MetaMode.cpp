// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagers/MetaMode.h"
#include "RacingSystem.h"
#include "PlaneRacingGame/RaceCheckpoint.h"
#include "Kismet/GameplayStatics.h"
#include "GameManagers/SoundManager.h"
#include "MathMiniGame/MathGameSystem.h"

AMetaMode::AMetaMode()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMetaMode::BeginPlay()
{
	Super::BeginPlay();
	
	WorldRacingSystem = Cast<ARacingSystem>(UGameplayStatics::GetActorOfClass(this, RacingSystemClass));
	WorldSoundManager = Cast<ASoundManager>(UGameplayStatics::GetActorOfClass(this, SoundManagerClass));
	WorldMathSystem = Cast<AMathGameSystem>(UGameplayStatics::GetActorOfClass(this, MathGameSystemClass));
	SetupPlaneRaceSystem();
}

void AMetaMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMetaMode::SetupPlaneRaceSystem()
{
	if (WorldRacingSystem)
	{
		
		for (int32 j = 0; j < WorldRacingSystem->GetRouteAmount(); j++)
		{
			int32 i = 0;
			for (ARaceCheckpoint* _Checkpoint : WorldRacingSystem->GetRacingCheckpoints(j))
			{
				if (!_Checkpoint)
					return;
				_Checkpoint->SetWorldRacingSystem(WorldRacingSystem);
				_Checkpoint->SetCheckpointIndex(i);
				i++;
			}
		}
	}
}
