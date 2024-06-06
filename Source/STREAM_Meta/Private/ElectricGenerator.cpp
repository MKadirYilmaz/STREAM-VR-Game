// All rights belong to Kadir Yilmaz


#include "ElectricGenerator.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/PointLightComponent.h"
#include "PhysicalButton.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AElectricGenerator::AElectricGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	FanMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FanMesh"));
	FanBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FanBody"));

	FanConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FanConstraint"));

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));

	ScreenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScreenMesh"));
	BatteryPercentageRenderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BatteryPercentegeRenderer"));

	RootComponent = CustomRoot;
	FanMesh->SetupAttachment(RootComponent);
	FanBody->SetupAttachment(RootComponent);
	ScreenMesh->SetupAttachment(RootComponent);
	BatteryPercentageRenderer->SetupAttachment(ScreenMesh);
	PointLightComponent->SetupAttachment(RootComponent);
	FanConstraint->SetupAttachment(RootComponent);

	FanMesh->SetSimulatePhysics(true);

	FanConstraint->SetConstrainedComponents(FanBody, NAME_None, FanMesh, NAME_None);

}

// Called when the game starts or when spawned
void AElectricGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	if (TiedButton)
	{
		TiedButton->ButtonPressed.AddDynamic(this, &AElectricGenerator::OnOffSwitch);
	}
	
	PointLightComponent->SetVisibility(OnOffCondition);
}

// Called every frame
void AElectricGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float _Change = FMath::Abs(LastTickRotation.Pitch - FanMesh->GetRelativeRotation().Pitch);
	StoredVoltageAmount += _Change * VoltageProductionMultiplier;
	if (IsConsuming)
	{
		PointLightComponent->SetIntensity(StoredVoltageAmount);
		StoredVoltageAmount -= VoltageConsumeMultiplier * PointLightComponent->Intensity * DeltaTime;
	}
	StoredVoltageAmount = FMath::Clamp(StoredVoltageAmount, 0.f, MaxStoredVoltage);
	if (StoredVoltageAmount <= 1.f)
	{
		OnOffSwitch(0); // Voltage 0'a cok yaklasirsa otomatik kapanir
	}

	//UE_LOG(LogTemp, Display, TEXT("Change: %f, Stored: %f, LT: %s, CT: %s"), _Change, StoredVoltageAmount, *LastTickRotation.ToString(), *FanMesh->GetRelativeRotation().ToString());
	SetScreenText((StoredVoltageAmount / MaxStoredVoltage) * 100);
	LastTickRotation = FanMesh->GetRelativeRotation();
}

void AElectricGenerator::OnOffSwitch(int32 ButtonIndex)
{
	if (OnOffCondition) // Acikken kapanirsa
	{
		PointLightComponent->SetVisibility(false);
		IsConsuming = false;
		OnOffCondition = false;

	}
	else // Kapaliyken acilirsa
	{
		PointLightComponent->SetVisibility(true);
		IsConsuming = true;
		OnOffCondition = true;
	}
}

