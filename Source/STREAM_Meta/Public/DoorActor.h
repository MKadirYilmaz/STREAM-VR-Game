// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorActor.generated.h"

UCLASS()
class STREAM_META_API ADoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	USceneComponent* EmptyRoot;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* DoorMesh;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* DoorHingeMesh;
	UPROPERTY(EditAnywhere)
		class UPhysicsConstraintComponent* DoorHinge;
};
