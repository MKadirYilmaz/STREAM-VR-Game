// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LifterSystem.generated.h"

UCLASS()
class STREAM_META_API ALifterSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALifterSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void MoveLifter();
	UFUNCTION(BlueprintImplementableEvent)
	void LifterMovingAnim();
	UFUNCTION(BlueprintImplementableEvent)
	void LifterStoppingAnim();
	UFUNCTION(BlueprintImplementableEvent)
	void HighlightPressedButton(int32 PressedIndex);
private:
	UFUNCTION()
	void CallLifter(int32 CalledFloor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* LifterMesh;

	UPROPERTY(EditAnywhere)
	TArray<class APhysicalButton*> TiedButtons;

	UPROPERTY(EditAnywhere)
	TArray<int32> FloorStopZAxis;
	UPROPERTY(EditAnywhere)
	TArray<class APhysicalButton*> LifterCallers;

	UPROPERTY(EditAnywhere)
	int32 TargetFloor;
	UPROPERTY(VisibleAnywhere)
	int32 CurrentFloor;

	UPROPERTY(EditAnywhere)
	float LifterSpeed = 3.f;

	float LifterDirection;

	FVector TargetLocation;

	bool ShouldMove = false;

	UFUNCTION()
	void ButtonSystem(int32 FloorIndex);

	TArray<int32> TargetFloors;
	void AddToQueue(int32 NewFloorIndex);
	
public:
	UFUNCTION(BlueprintCallable)
	void SetShouldMove(bool bShouldMove) { ShouldMove = bShouldMove; }
};
