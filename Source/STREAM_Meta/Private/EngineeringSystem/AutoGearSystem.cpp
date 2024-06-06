// All rights belong to Muhammed Kadir Yilmaz


#include "EngineeringSystem/AutoGearSystem.h"
#include "MechanicWheel.h"

// Sets default values for this component's properties
UAutoGearSystem::UAutoGearSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAutoGearSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAutoGearSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentTireSpeed > FirstGearMaxSpeed) // Motor ilk vites max hizindan hizliysa
	{
		if (CurrentTireSpeed > SecondGearMaxSpeed) // Motor ikinci vites max hizindan hizliysa
		{
			if (CurrentTireSpeed >= ThirdGearMaxSpeed) // Hizi sinirla
			{
				//CurrentEngineSpeed = FMath::Clamp(CurrentEngineSpeed, 0, ThirdGearMaxSpeed);
			}
			else if(CurrentGearIndex != 3) // Ucuncu vitese gec
			{
				SwitchToThirdGear();
			}
		}
		else if(CurrentGearIndex != 2) // Ikinci vitese gec
		{
			SwitchToSecondGear();
		}
	}
	else if(CurrentGearIndex != 1) // Ilk vitese gec 
	{ 
		SwitchToFirstGear();
	}
}

void UAutoGearSystem::UpdateEngineSpeed(float EngineSpeed)
{
	CurrentEngineSpeed = EngineSpeed;
}

void UAutoGearSystem::SwitchToFirstGear()
{
	if (CurrentGearIndex == 2) // Yalnizca 2'den 1'e inilebilir
	{
		Gear2Connection->SetChildWheelComp(nullptr);
		Gear1Connection->SetChildWheelComp(GearToWheel);

		CurrentEngineSpeed *= 2;
	}
	GearBoxToOne();
	CurrentGearIndex = 1;
}

void UAutoGearSystem::SwitchToSecondGear()
{
	if (CurrentGearIndex == 1) // 1'den 2'ye cikiliyorsa
	{
		Gear1Connection->SetChildWheelComp(nullptr);
		Gear2Connection->SetChildWheelComp(GearToWheel);

		CurrentEngineSpeed *= 0.5f;
	}
	else if (CurrentGearIndex == 3) // 3'den 2'ye iniliyorsa
	{
		Gear3Connection->SetChildWheelComp(nullptr);
		Gear2Connection->SetChildWheelComp(GearToWheel);
		CurrentEngineSpeed *= 2.f;
	}
	GearBoxToTwo();
	CurrentGearIndex = 2;
}

void UAutoGearSystem::SwitchToThirdGear()
{
	if (CurrentGearIndex == 2) // Yalnizca 2'den 3'e cikilabilir
	{
		Gear2Connection->SetChildWheelComp(nullptr);
		Gear3Connection->SetChildWheelComp(GearToWheel);

		CurrentEngineSpeed *= 0.5f;
	}
	GearBoxToThree();
	CurrentGearIndex = 3;
}

