// All rights belong to Kadir Yilmaz


#include "BowlingLabuts.h"
#include "PhysicalButton.h"

// Sets default values
ABowlingLabuts::ABowlingLabuts()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LabutsRoot = CreateDefaultSubobject<USceneComponent>(TEXT("LabutsRoot"));
	RootComponent = LabutsRoot;
	/*
	for (int32 i = 0; i < Labuts.Num(); i++)
	{
		Labuts[i] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Labut" + i));
		if (Labuts[i])
		{
			Labuts[i]->SetupAttachment(RootComponent);
			Labuts[i]->SetSimulatePhysics(true);
		}
	}
	*/

	Labut1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Labut1"));
	Labut2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Labut2"));
	Labut3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Labut3"));
	Labut4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Labut4"));
	Labut5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Labut5"));
	Labut6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Labut6"));
	Labut7 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Labut7"));
	Labut8 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Labut8"));
	Labut9 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Labut9"));
	Labut10 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Labut10"));

	Labut1->SetupAttachment(RootComponent);
	Labut2->SetupAttachment(RootComponent);
	Labut3->SetupAttachment(RootComponent);
	Labut4->SetupAttachment(RootComponent);
	Labut5->SetupAttachment(RootComponent);
	Labut6->SetupAttachment(RootComponent);
	Labut7->SetupAttachment(RootComponent);
	Labut8->SetupAttachment(RootComponent);
	Labut9->SetupAttachment(RootComponent);
	Labut10->SetupAttachment(RootComponent);

	Labut1->SetSimulatePhysics(true);
	Labut2->SetSimulatePhysics(true);
	Labut3->SetSimulatePhysics(true);
	Labut4->SetSimulatePhysics(true);
	Labut5->SetSimulatePhysics(true);
	Labut6->SetSimulatePhysics(true);
	Labut7->SetSimulatePhysics(true);
	Labut8->SetSimulatePhysics(true);
	Labut9->SetSimulatePhysics(true);
	Labut10->SetSimulatePhysics(true);
	
	Labuts.Empty();
	Labuts.Add(Labut1);
	Labuts.Add(Labut2);
	Labuts.Add(Labut3);
	Labuts.Add(Labut4);
	Labuts.Add(Labut5);
	Labuts.Add(Labut6);
	Labuts.Add(Labut7);
	Labuts.Add(Labut8);
	Labuts.Add(Labut9);
	Labuts.Add(Labut10);

}

// Called when the game starts or when spawned
void ABowlingLabuts::BeginPlay()
{
	Super::BeginPlay();
	
	for (UStaticMeshComponent* _Labut : Labuts)
	{
		if (_Labut)
		{
			LabutDefaultTransforms.Add(_Labut->GetComponentTransform());
			LabutsShouldReturn.Add(false);
		}
	}
	if (TiedButton)
	{
		TiedButton->ButtonPressed.AddDynamic(this, &ABowlingLabuts::ResetLabutTransforms);
	}
}

// Called every frame
void ABowlingLabuts::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!ShouldReturn)
		return;
	for (int32 i = 0; i < Labuts.Num(); i++)
	{
		if (i < LabutDefaultTransforms.Num() && Labuts[i] && LabutsShouldReturn[i])
		{
			Labuts[i]->SetWorldLocationAndRotation(FMath::VInterpTo(Labuts[i]->GetComponentLocation(), LabutDefaultTransforms[i].GetLocation(), DeltaTime, 4.f),
				FMath::RInterpTo(Labuts[i]->GetComponentRotation(), LabutDefaultTransforms[i].GetRotation().Rotator(), DeltaTime, 4.f));
			if (FVector::Distance(Labuts[i]->GetComponentLocation(), LabutDefaultTransforms[i].GetLocation()) < 10.f && Labuts[i]->GetComponentRotation().Equals(LabutDefaultTransforms[i].GetRotation().Rotator(), 1)) 
			{
				LabutsShouldReturn[i] = false;
				
				if (!CheckAllLabutsShouldReturn())
				{
					FTimerHandle LabutTimer;
					GetWorldTimerManager().SetTimer(LabutTimer, this, &ABowlingLabuts::EnableLabutSimulation, 0.5f, false);
					ShouldReturn = false;
				}
			}
		}
	}
}

void ABowlingLabuts::ResetLabutTransforms(int32 ButtonIndex)
{
	SetLabutSimulation(false);
	for (int32 i = 0; i < LabutsShouldReturn.Num(); i++)
	{
		LabutsShouldReturn[i] = true;
	}
	ShouldReturn = true;
}

void ABowlingLabuts::SetLabutSimulation(bool bPhysicsEnabled)
{
	for (UStaticMeshComponent* _Labut : Labuts)
	{
		if (_Labut)
		{
			_Labut->SetSimulatePhysics(bPhysicsEnabled);
			_Labut->ComponentVelocity = FVector(0, 0, 0);
		}
	}
}

void ABowlingLabuts::EnableLabutSimulation()
{
	SetLabutSimulation(true);
}

bool ABowlingLabuts::CheckAllLabutsShouldReturn()
{
	for (bool _ShouldReturn : LabutsShouldReturn)
	{
		if (_ShouldReturn)
			return true;
	}
	return false;
}


