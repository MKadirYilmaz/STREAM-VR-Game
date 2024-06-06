// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleSystem/BasicTrain.h"
#include "Components/SplineComponent.h"
#include "InteractionScripts/LeverSystem.h"
#include "PhysicalButton.h"

// Sets default values
ABasicTrain::ABasicTrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TrainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrainMesh"));
}

// Called when the game starts or when spawned
void ABasicTrain::BeginPlay()
{
	Super::BeginPlay();
	if(TrainRoadActor)
		TrainRoad = TrainRoadActor->GetComponentByClass<USplineComponent>();

	

	if (!TrainParts.IsEmpty() && TrainRoad)
	{
		DistanceBetweenParts.Empty();
		TrainPartDistances.SetNum(TrainParts.Num());
		for (int32 i = 0; i < TrainParts.Num() - 1; i++) // Find the distances between each parts
		{
			DistanceBetweenParts.Add(FVector::Distance(TrainParts[i]->GetComponentLocation(), TrainParts[i + 1]->GetComponentLocation()));
		}
		float _Distance = TrainStartDistance;
		TrainParts[TrainParts.Num() - 1]->SetWorldLocation(TrainRoad->GetLocationAtDistanceAlongSpline(_Distance, ESplineCoordinateSpace::World)); // Set last part's location based on start distance
		TrainPartDistances[TrainParts.Num() - 1] = _Distance; // Last part's starting distance is start distance
		int32 j = DistanceBetweenParts.Num() - 1;
		for (int32 i = TrainParts.Num() - 2; i >= 0; i--) // Set each train part's starting location ( (num - 1)-- (num - 2)-- (num - 3)-- )
		{
			if (j >= 0)
			{
				_Distance += DistanceBetweenParts[j];
				j--;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("DistanceBetweenParts array is not long enough (BasicTrain)"));
			}
			TrainParts[i]->SetWorldLocation(TrainRoad->GetLocationAtDistanceAlongSpline(_Distance, ESplineCoordinateSpace::World));
			TrainPartDistances[i] = _Distance;
		}
	}
	
	StationCall.Empty();
	for (int32 i = 0; i < StationDistances.Num(); i++)
	{
		StationCall.Add(false);
	}
	for (APhysicalButton* _Button : TiedStationButtons)
	{
		if (_Button)
			_Button->ButtonPressed.AddDynamic(this, &ABasicTrain::CallTrainFromStation);
	}
	
}

// Called every frame
void ABasicTrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!TrainRoad)
	{
		UE_LOG(LogTemp, Error, TEXT("TrainRoad is nullptr"));
		return;
	}
	
	if (!TrainCalled) // Train cagrilmamissa
	{
		float _FrontValue = (HeadLever) ? HeadLever->GetCurrentLeverValue() + 1.f : 0.f;
		float _TailValue = (TailLever) ? TailLever->GetCurrentLeverValue() - 1.f : 0.f;
		TrainControlLeversValue = FMath::FInterpTo(TrainControlLeversValue, _FrontValue + _TailValue, DeltaTime, TrainAccelerationSpeed); // -2 -> 2
	}
	else // Tren cagrilmissa
	{
		if((TrainPartDistances[0] < TargetDistance && TrainPartDistances[TrainPartDistances.Num() - 1] > TargetDistance) || 
			(TrainPartDistances[0] > TargetDistance && TrainPartDistances[TrainPartDistances.Num() - 1] < TargetDistance)) // Trenin basi veya sonu TargetDistance'i biraz gecmisse
		{
			UE_LOG(LogTemp, Warning, TEXT("Train is now at the station. Slowing down"));
			TrainControlLeversValue = FMath::FInterpTo(TrainControlLeversValue, 0.f, DeltaTime, TrainAccelerationSpeed);
			if (HeadLever && TailLever)
			{
				HeadLever->SetLeverPitchValue((TrainControlLeversValue * 50.f) / 2);
				TailLever->SetLeverPitchValue((TrainControlLeversValue * 50.f) / 2);
			}
			if (FMath::Abs(TrainControlLeversValue) < 0.01f)
			{
				UE_LOG(LogTemp, Display, TEXT("Train is now at the station. Stopped"));
				TrainCalled = false;
				if(CalledStation < StationCall.Num())
					StationCall[CalledStation] = false;
				if (HeadLever && TailLever)
				{
					HeadLever->SetLeverPitchValue(0.f);
					TailLever->SetLeverPitchValue(0.f);
					HeadLever->SetHoldable(true);
					TailLever->SetHoldable(true);
				}
			}
		}
	}
	SetIndividualTrainPartLocation(TrainMaxSpeed * DeltaTime * TrainControlLeversValue);
	//UE_LOG(LogTemp, Display, TEXT("TrainDistance: %f"), TrainPartDistances[0]);
}

FVector ABasicTrain::GetLocomotiveWorldLocation(int32 LocomotiveIndex)
{
	if (TrainRoad)
		return TrainRoad->GetLocationAtDistanceAlongSpline(DistanceMoved - (DistanceBetweenLocomotives * LocomotiveIndex), ESplineCoordinateSpace::World);
	else
		return FVector::ZeroVector;
}

FRotator ABasicTrain::GetLocomotiveWorldRotation(int32 LocomotiveIndex)
{
	if(TrainRoad)
		return TrainRoad->GetRotationAtDistanceAlongSpline(DistanceMoved - (DistanceBetweenLocomotives * LocomotiveIndex), ESplineCoordinateSpace::World);
	else
		return FRotator::ZeroRotator;
}

void ABasicTrain::CallTrainFromStation(int32 StationIndex)
{
	if (StationIndex < StationCall.Num() && StationDistances.Num() > StationIndex)
	{
		StationCall[StationIndex] = true;
		TrainCalled = true;
		TargetDistance = StationDistances[StationIndex];
		CalledStation = StationIndex;
		if (TrainPartDistances[0] > TargetDistance && TrainPartDistances[TrainPartDistances.Num() - 1] > TargetDistance) // Trenin basi ve sonu istasyonu gecmisse
		{
			UE_LOG(LogTemp, Display, TEXT("Train is passed the station. Now returning"));
			TrainControlLeversValue = -2.f;
			if (HeadLever && TailLever)
			{
				HeadLever->SetHoldable(false);
				TailLever->SetHoldable(false);
			}
		}
		else if (TrainPartDistances[0] < TargetDistance && TrainPartDistances[TrainPartDistances.Num() - 1] < TargetDistance) // Trenin basi ve sonu istasyonun gerisinde ise
		{
			UE_LOG(LogTemp, Display, TEXT("Station in front of the train. Now moving"));
			TrainControlLeversValue = 2.f;
			if (HeadLever && TailLever)
			{
				HeadLever->SetHoldable(false);
				TailLever->SetHoldable(false);
			}
		}
		else // Istasyon trenin basi ile sonu arasinda bir yerde ise
		{
			UE_LOG(LogTemp, Warning, TEXT("Train is already at the station"));
			if (HeadLever && TailLever)
			{
				HeadLever->SetLeverPitchValue(0.f);
				TailLever->SetLeverPitchValue(0.f);
			}
			TrainCalled = false;
			StationCall[StationIndex] = false;
		}
	}
}

void ABasicTrain::SetIndividualTrainPartLocation(float MovedDistance)
{
	if (TrainRoad)
	{
		if (MovedDistance > 0)
		{
			if (TrainPartDistances[0] + MovedDistance < TrainRoad->GetSplineLength())
			{
				for (int32 i = 0; i < TrainParts.Num(); i++)
				{
					TrainPartDistances[i] += MovedDistance;
					TrainParts[i]->SetWorldRotation(TrainRoad->GetRotationAtDistanceAlongSpline(TrainPartDistances[i], ESplineCoordinateSpace::World));
					TrainParts[i]->SetWorldLocation(TrainRoad->GetLocationAtDistanceAlongSpline(TrainPartDistances[i], ESplineCoordinateSpace::World) 
						+ TrainParts[i]->GetUpVector() * RailHeight);
				}
			}
		}
		else if (MovedDistance < 0) // Moving backwards
		{
			if (TrainPartDistances[TrainPartDistances.Num() - 1] + MovedDistance > 0)
			{
				for (int32 i = 0; i < TrainParts.Num(); i++)
				{
					TrainPartDistances[i] += MovedDistance;
					TrainParts[i]->SetWorldRotation(TrainRoad->GetRotationAtDistanceAlongSpline(TrainPartDistances[i], ESplineCoordinateSpace::World));
					TrainParts[i]->SetWorldLocation(TrainRoad->GetLocationAtDistanceAlongSpline(TrainPartDistances[i], ESplineCoordinateSpace::World)
						+ TrainParts[i]->GetUpVector() * RailHeight);
				}
			}
		}
	}
}


