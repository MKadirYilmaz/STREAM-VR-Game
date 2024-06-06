// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaceCheckpoint.generated.h"

UCLASS()
class STREAM_META_API ARaceCheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARaceCheckpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets visibility and activation of the checkpoint
	void SetActiveCheckpoint(bool bActive);
private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* CheckPointMesh;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MiddleZone;

	class ARacingSystem* RacingSystem;
	UPROPERTY(VisibleAnywhere)
	int32 CheckpointIndex;

	bool IsCheckpointActive = true;
public:
	void SetWorldRacingSystem(class ARacingSystem* WRS) { RacingSystem = WRS; }
	void SetCheckpointIndex(int32 Index) { CheckpointIndex = Index; }

};
