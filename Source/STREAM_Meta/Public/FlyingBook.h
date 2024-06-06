// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlyingBook.generated.h"

UCLASS()
class STREAM_META_API AFlyingBook : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlyingBook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	void MoveBook(float DeltaSeconds);
	
	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* BookMesh;
	UPROPERTY(EditAnywhere)
	class USplineComponent* BookPath;


	UPROPERTY(EditAnywhere)
	float NormalBookSpeed = 500.f;
	UPROPERTY(EditAnywhere)
	float AnimationSpeedMultiplier = 1.25f;
	float Distance;

	UPROPERTY(EditAnywhere)
	UAnimationAsset* BookFlyAnim;

};
