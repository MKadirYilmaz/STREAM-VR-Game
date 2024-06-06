// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionScripts/PhysicsObjects.h"
#include "PhysicsWheel.generated.h"

/**
 * 
 */
UCLASS()
class STREAM_META_API APhysicsWheel : public APhysicsObjects
{
	GENERATED_BODY()

public:
	APhysicsWheel();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float GetWheelAngularVelocity();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* WheelMesh;
	UPROPERTY(EditAnywhere)
		class UPhysicsConstraintComponent* WheelConstraint;

	FRotator StartLocalRotation;
	float TotalRotationChange;

	UPROPERTY(EditAnywhere)
		class ABaseCar* TiedBaseCar;


public:

};
