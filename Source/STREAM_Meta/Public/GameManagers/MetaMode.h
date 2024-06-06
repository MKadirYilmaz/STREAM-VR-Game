// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MetaMode.generated.h"

/**
 * 
 */
class ASoundManager;
class ARacingSystem;
class AMathGameSystem;
UCLASS()
class STREAM_META_API AMetaMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMetaMode();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	void SetupPlaneRaceSystem();

	ARacingSystem* WorldRacingSystem;
	ASoundManager* WorldSoundManager;
	AMathGameSystem* WorldMathSystem;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ARacingSystem> RacingSystemClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ASoundManager> SoundManagerClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AMathGameSystem> MathGameSystemClass;
public:
	ARacingSystem* GetWorldRacingSystem() { return WorldRacingSystem; }
	ASoundManager* GetWorldSoundManager() { return WorldSoundManager; }
	AMathGameSystem* GetWorldMathGameSystem() { return WorldMathSystem; }
	
};
