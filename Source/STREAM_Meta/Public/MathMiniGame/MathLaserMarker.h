// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionScripts/PhysicsObjects.h"
#include "MathLaserMarker.generated.h"

class AMathBomb;

UCLASS()
class STREAM_META_API AMathLaserMarker : public APhysicsObjects
{
	GENERATED_BODY()

public:
	AMathLaserMarker();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void LockOnBomb();

protected:

	virtual void BeginPlay() override;


protected:
	UPROPERTY(EditAnywhere)
	class AMathGameSystem* WorldMathSystem;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LaserMesh;
	UPROPERTY(EditAnywhere)
	USceneComponent* LaserEndMarker;

	AMathBomb* ScannedBomb;
	FHitResult MathHitResult;
	
	float InitialLaserScale;

	UPROPERTY(EditAnywhere)
	float LaserMaxDistance = 1000.f;
	UPROPERTY(EditAnywhere)
	float LaserLengthMultiplier = 1.5f;
};
