// All rights belong to Muhammed Kadir Yilmaz


#include "MechanicWheel.h"
#include "InteractionScripts/LeverSystem.h"

UMechanicWheel::UMechanicWheel()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMechanicWheel::BeginPlay()
{
	Super::BeginPlay();

}

void UMechanicWheel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (StartingWheel) // If the gear wheel is the one who connects directly to the engine
	{
		DirectEngine();
		if (TiedEngineSpeedLever && TiedEnginePowerLever)
		{
			RotationSpeed += TiedEngineSpeedLever->GetCurrentLeverValue() * EngineAccelerationSpeed;
			RotationSpeed = FMath::Clamp(RotationSpeed, EngineMaxBackwardsSpeed, EngineMaxForwardSpeed);
			
			WheelPower += TiedEnginePowerLever->GetCurrentLeverValue() * (EngineAccelerationSpeed / 2.f);
			WheelPower = FMath::Clamp(WheelPower, EngineMaxBackwardsTorque, EngineMaxForwardTorque);
		}
	}
	if (ChildWheel && NumOfTeeth != 0)
	{
		if (!TiedWithChild)
		{
			ChildWheel->WheelPower = (WheelPower * ChildWheel->NumOfTeeth) / NumOfTeeth;
			ChildWheel->RotationSpeed = -(RotationSpeed * NumOfTeeth) / ChildWheel->NumOfTeeth;
			//UE_LOG(LogTemp, Display, TEXT("%s, Speed %f"), *ChildWheel->GetName(), ChildWheel->RotationSpeed);
		}
		else
		{
			ChildWheel->WheelPower = WheelPower;
			ChildWheel->RotationSpeed = RotationSpeed;
		}
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("%s Nullptr error!!"), *GetName());
	}
	if (!AdditionalChilds.IsEmpty() && NumOfTeeth != 0)
	{
		for (UMechanicWheel* _MG : AdditionalChilds)
		{
			_MG->WheelPower = (WheelPower * _MG->NumOfTeeth) / NumOfTeeth;
			_MG->RotationSpeed = -(RotationSpeed * NumOfTeeth) / _MG->NumOfTeeth;
			//UE_LOG(LogTemp, Display, TEXT("%s, Speed %f"), *_MG->GetName(), _MG->RotationSpeed);
		}
	}
	AddLocalRotation(FRotator(0, 0, RotationSpeed * DeltaTime));
}

void UMechanicWheel::GetEngineProperties(float EngineSpeed, float EnginePower)
{
	RotationSpeed = EngineSpeed;
	WheelPower = EnginePower;
}

void UMechanicWheel::SetFirstAdditionalChild(UMechanicWheel* NewChild)
{
	if (NewChild == nullptr)
	{
		if(!AdditionalChilds.IsEmpty())
			AdditionalChilds.RemoveAt(0);
	}
	else
	{
		if (!AdditionalChilds.IsEmpty())
			AdditionalChilds[0] = NewChild;
		else
			AdditionalChilds.Add(NewChild);
	}
}

void UMechanicWheel::SetAdditionalChildComps(TArray<UMechanicWheel*> Childs)
{
	AdditionalChilds = Childs;
}
