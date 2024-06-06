// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCar.generated.h"

enum ECarMoveStates
{
	Stopping,
	MovingForward,
	MovingBackwards,
	Breaking
};

class UCarTireComponent;
UCLASS()
class STREAM_META_API ABaseCar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseCar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	FVector GetCarSpeedVector(float ForwardValue, float DeltaSeconds);
	FVector GetSurfaceNormal();
	void AlignCarWithSurface(const FVector& SurfaceNormal, float DeltaSeconds);
	void RecalculateAccelerationForSlope(const ECarMoveStates& MovementState, float& OutCurrentAcceleration, float& OutCurrentMaxSpeed);
	void SlopeRecalculations(int32 MovementDirection, float& OutCurrentAcceleration, float& OutCurrentMaxSpeed);

	ECarMoveStates CurrentMovementState = Stopping;

	UPROPERTY(EditAnywhere)
	float ForwardMaxSpeed = 30.f;
	UPROPERTY(EditAnywhere)
	float BackwardsMaxSpeed = 15.f;
	UPROPERTY(EditAnywhere)
	float ForwardAcceleration = 0.25f;
	UPROPERTY(EditAnywhere)
	float BreakingAcceleration = 1.f;
	UPROPERTY(EditAnywhere)
	float BackwardsAcceleration = 0.2f;
	UPROPERTY(EditAnywhere)
	float StoppingAcceleration = 0.3f;
	float MaxSpeed = ForwardMaxSpeed;
	float CurrentSpeed = 0.f;
	float CurrentCarAcceleration = ForwardAcceleration;


	USceneComponent* CarRootComp;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BaseCarCollision;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* CarBodyMesh;
	UPROPERTY(EditAnywhere)
		UCarTireComponent* FrontRight;
	UPROPERTY(EditAnywhere)
		UCarTireComponent* FrontLeft;
	UPROPERTY(EditAnywhere)
		UCarTireComponent* BehindRight;
	UPROPERTY(EditAnywhere)
		UCarTireComponent* BehindLeft;
	UPROPERTY(EditAnywhere)
		USceneComponent* SeatConnector;

	class APCharacter* Player;

	int32 RightLeftValue;

public:
	UCarTireComponent* GetFrontRightTire() { return FrontRight; }
	UCarTireComponent* GetFrontLeftTire() { return FrontLeft; }

	void SetRightLeftValue(int32 RightLeft) { this->RightLeftValue = RightLeft; }
};
