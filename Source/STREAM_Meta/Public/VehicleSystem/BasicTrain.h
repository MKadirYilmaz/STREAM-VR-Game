// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicTrain.generated.h"

UCLASS()
class STREAM_META_API ABasicTrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicTrain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	FVector GetLocomotiveWorldLocation(int32 LocomotiveIndex);

	UFUNCTION(BlueprintCallable)
	FRotator GetLocomotiveWorldRotation(int32 LocomotiveIndex);
	
	UFUNCTION(BlueprintImplementableEvent)
	void RecalculateLocomotiveTransforms();

	UFUNCTION(BlueprintCallable)
	void CallTrainFromStation(int32 StationIndex);
private:
	void SetIndividualTrainPartLocation(float MovedDistance);

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TrainMesh;

	UPROPERTY(EditAnywhere)
	AActor* TrainRoadActor;

	class USplineComponent* TrainRoad;

	UPROPERTY(EditAnywhere)
	class ALeverSystem* HeadLever;

	UPROPERTY(EditAnywhere)
	class ALeverSystem* TailLever;

	UPROPERTY(EditAnywhere)
	float DistanceBetweenLocomotives = 20.f;

	float CurrentSpeed = 100.f;
	float DistanceMoved = 0.f;

	UPROPERTY(EditAnywhere)
	float RailHeight = 50.f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TrainMaxSpeed = 300.f;
	UPROPERTY(EditAnywhere)
	float TrainAccelerationSpeed = 2.f;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAcess = "true"))
	TArray<UStaticMeshComponent*> TrainParts;
	UPROPERTY(EditAnywhere)
	TArray<class APhysicalButton*> TiedStationButtons;

	UPROPERTY(VisibleAnywhere)
	TArray<float> DistanceBetweenParts;
	UPROPERTY(VisibleAnywhere)
	TArray<float> TrainPartDistances;

	float TrainLength;
	float TrainControlLeversValue;

	UPROPERTY(EditAnywhere)
	float TrainStartDistance = 0.f;

	UPROPERTY(EditAnywhere)
	TArray<float> StationDistances;
	TArray<bool> StationCall;
	int32 CalledStation;
	bool TrainCalled = false;
	float TargetDistance;
};
