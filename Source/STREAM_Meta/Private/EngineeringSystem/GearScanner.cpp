// All rights belong to Muhammed Kadir Yilmaz


#include "EngineeringSystem/GearScanner.h"
#include "MechanicWheel.h"

AGearScanner::AGearScanner()
{
	PrimaryActorTick.bCanEverTick = true;

	LaserStart = CreateDefaultSubobject<USceneComponent>(TEXT("LaserStart"));
	LaserEnd = CreateDefaultSubobject<USceneComponent>(TEXT("LaserEnd"));

	LaserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserMesh"));

	LaserStart->SetupAttachment(Mesh);
	LaserEnd->SetupAttachment(Mesh);

	LaserMesh->SetupAttachment(Mesh);
	LaserMesh->SetCollisionProfileName(FName("OverlapAll"));
}

void AGearScanner::BeginPlay()
{
	Super::BeginPlay();
	InitialLaserScale = FVector::Distance(LaserStart->GetComponentLocation(), LaserEnd->GetComponentLocation());
}

void AGearScanner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult _HitResult;
	FVector _Start = LaserStart->GetComponentLocation();
	FVector _End = _Start + LaserStart->GetUpVector() * MaxLaserDistance;
	bool _Hitted = GetWorld()->LineTraceSingleByChannel(_HitResult, _Start, _End, ECC_Visibility);
	if (_Hitted)
	{
		if(InitialLaserScale != 0)
			LaserMesh->SetWorldScale3D(FVector(0.05f, 0.05f, _HitResult.Distance / InitialLaserScale));

		ScannedGear = Cast<UMechanicWheel>(_HitResult.GetComponent());
		if (ScannedGear)
		{
			UE_LOG(LogTemp, Display, TEXT("Power %f, RotationSpeed %f"), ScannedGear->GetWheelPower(), ScannedGear->GetWheelSpeed());
			SetGearToScreen(ScannedGear);
		}
	}
	else
	{
		if (InitialLaserScale != 0)
			LaserMesh->SetWorldScale3D(FVector(0.05f, 0.05f, MaxLaserDistance / InitialLaserScale));
	}
}
