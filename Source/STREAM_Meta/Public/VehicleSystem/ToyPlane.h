// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToyPlane.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlaneHit);

class UCameraComponent;
UCLASS()
class STREAM_META_API AToyPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToyPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FOnPlaneHit OnPlaneHit;

	void SwitchCamera(bool ToPlane);
	UFUNCTION(BlueprintImplementableEvent)
	void TransitionToCamera(AActor* TargetCameraActor, APlayerController* PlayerController);
	UFUNCTION(BlueprintImplementableEvent)
	void SoundSystem(bool bStart);
	
	void CalculateDesiredDirection(float DeltaSeconds);
	void RotatePlane(float RightValue, float UpValue, float DeltaSeconds);
	UFUNCTION(BlueprintCallable)
	void RotateHorizontally(float YawValue, float DeltaSeconds);

	void ReturnToStartingPoint();

	UFUNCTION(BlueprintCallable)
	void StartFlying();

	UFUNCTION(BlueprintCallable)
	void StopFlying();

	UFUNCTION(BlueprintCallable)
	void SpeedChange(float Value);

	UFUNCTION(BlueprintCallable)
	void ReversePlaneRotation(bool IsReverse);

	UFUNCTION(BlueprintCallable)
	void ForceStopPlane();

	UFUNCTION(BlueprintCallable)
	void DebugMoveUpwards(float Value);

	UFUNCTION(BlueprintCallable)
	void DebugMoveRight(float Value);

	UFUNCTION(BlueprintCallable)
	void AbortPlaneFlight();

	bool CollisionBroadcasted = false;
private:
	UFUNCTION()
		void RotateCameraUp(float Value);
	UFUNCTION()
		void RotateCameraRight(float Value);
	UFUNCTION()
		void GetForwardValue(float Value);
	UFUNCTION()
		void GetRightValue(float Value);

	void RotateMotor(float DeltaSeconds);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BasePlaneMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* PlaneCameraRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* PlaneCamera;
	UPROPERTY(EditAnywhere)
		USceneComponent* PlaneRestartPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MotorMesh;

	UPROPERTY(EditAnywhere)
		float MaxSpeed = 3000.f;
	UPROPERTY(EditAnywhere)
		float PitchRotSpeed = 75.f;
	UPROPERTY(EditAnywhere)
		float RollRotSpeed = 125.f;
	UPROPERTY(EditAnywhere)
	float YawRotSpeed = 37.5f;

	UPROPERTY(EditAnywhere)
		float PitchInterpSpeed = 1.4f;
	UPROPERTY(EditAnywhere)
		float RollInterpSpeed = 2.2f;
	UPROPERTY(EditAnywhere)
		float YawInterpSpeed = 0.55f;

	UPROPERTY(EditAnywhere)
		float AccelerationSpeed = 50.f;
	UPROPERTY(EditAnywhere)
	int32 PlaneRouteIndex = 0;

	UPROPERTY(EditAnywhere)
		float DirectionIntepSpeed = 2.5f;
	UPROPERTY(EditAnywhere)
		bool CanLookUp = false;

	float CurrentSpeed = 0.f;

	FVector DesiredDirection;
	FVector MovementDirection;

	FRotator RollRotationDelta;
	FRotator PitchRotationDelta;
	FRotator YawRotationDelta;

	class APCharacter* Player;
	class AVRC* VR_Player;
	bool FlightStarted = false;

	float DebugMovingSpeed = 200.f;
	bool DebugForceStop = false;
	UPROPERTY(EditAnywhere)
	bool IsPlaneActive = false;

	float ForwardInput, RightInput;
	FTransform StartingTransform;

	int32 PlaneRotationReverse = 1.f; // 1 Normal, -1 Reverse

public:
	float GetPlaneMaxSpeed() { return MaxSpeed; }
	UFUNCTION(BlueprintCallable)
	float GetPlaneCurrentSpeed() const { return CurrentSpeed; }
	UFUNCTION(BlueprintCallable)
	float GetCurrentTime() const;
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetFlightStarted() const { return FlightStarted; }
};
