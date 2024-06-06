// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTimer.h"

// Sets default values for this component's properties
UBaseTimer::UBaseTimer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBaseTimer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBaseTimer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//DeltaSeconds = DeltaTime;
	
	if (CounterStarted)
	{
		CurrentTime += DeltaTime;
	}
	if (CountdownStarted)
	{
		RemainedTime -= DeltaTime;
		if (RemainedTime <= 0.f)
		{
			StopCountdown.Broadcast(CountdownStarter);
			EndCountdown();
		}
	}
}

void UBaseTimer::StartCountdown(float Duration, AActor* CountdownOwner)
{
	CountdownStarter = CountdownOwner;
	RemainedTime = Duration;
	CountdownStarted = true;
}

void UBaseTimer::EndCountdown()
{
	CountdownStarted = false;
}

void UBaseTimer::StartCounter()
{
	CurrentTime = 0;
	CounterStarted = true;
	BreakingPoints.Empty();
}

float UBaseTimer::StopCounter()
{
	CounterStarted = false;
	return CurrentTime;
}

void UBaseTimer::AddBreakpoint(int32 Index)
{
	if (Index >= BreakingPoints.Num()) // Egerki array buyuklugu daha onceden ayarlanmamissa
		BreakingPoints.Add(CurrentTime);
	else // Array buyuklugu ayarlanmissa
		BreakingPoints[Index] = CurrentTime;
}

void UBaseTimer::SetBreakpointArraySize(int32 Size)
{
	BreakingPoints.SetNumZeroed(Size);
}

