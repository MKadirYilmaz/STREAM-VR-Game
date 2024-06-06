// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundManager.generated.h"

USTRUCT(BlueprintType)
struct FTagSounds
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FName CombinedTagNames;

	UPROPERTY(EditAnywhere)
	USoundBase* TagSound;
};

UCLASS()
class STREAM_META_API ASoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoundManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	TArray<FTagSounds> AllSoundFX;
	UPROPERTY(EditAnywhere)
	USoundBase* DefaultCollisionSound;

	UPROPERTY(EditAnywhere)
	float VolumeMultiplier = 1.f;


	USoundBase* GetSoundFromTag(FName SurfaceTag1, FName SurfaceTag2);

public:
	void CalculateAndPlaySound(FName Obj1SurfaceTag, FName Obj2SurfaceTag, FVector HitImpulse, FVector HitLocation);


};
