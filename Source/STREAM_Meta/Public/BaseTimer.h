// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseTimer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownFinished, AActor*, OwnerOfCountdown);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STREAM_META_API UBaseTimer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseTimer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartCountdown(float Duration, AActor* CountdownOwner);
	void EndCountdown();
	
	FOnCountdownFinished StopCountdown;

	void StartCounter();
	float StopCounter();

	void AddBreakpoint(int32 Index = 1000);
private:

	float DeltaSeconds = 0;
	float CurrentTime = 0;
	bool CounterStarted = false;

	TArray<float> BreakingPoints;

	float RemainedTime;
	bool CountdownStarted = false;
	AActor* CountdownStarter;
public:
	float GetCurrentTime() const { return CurrentTime; }
	TArray<float> GetBreakingPoints() const { return BreakingPoints; }

	float GetRemainedTime() { return RemainedTime; }

	void SetBreakpointArraySize(int32 Size);
};
