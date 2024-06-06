// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "CarTireComponent.generated.h"

/**
 * 
 */
UCLASS()
class STREAM_META_API UCarTireComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UCarTireComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FVector CurrentTireDirection;

public:
	void SetCurrentTireDirection(FVector TireDirection) { this->CurrentTireDirection = TireDirection; }
	FVector GetCurrentTireDirection() { return CurrentTireDirection; }
	
};
