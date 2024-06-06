// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaneControlStick.generated.h"

UCLASS()
class STREAM_META_API APlaneControlStick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaneControlStick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float GetPitchAngularDelta();
	float GetRollAngularDelta();

	FRotator StartLocalRotation;

	USceneComponent* DefaultRoot;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* ControlStickMesh;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* ConnectionMesh;
	UPROPERTY(EditAnywhere)
		class UPhysicsConstraintComponent* StickConstraint;

};
