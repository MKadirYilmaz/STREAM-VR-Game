// All rights belong to Muhammed Kadir Yilmaz


#include "MathMiniGame/TriangleCreator.h"
#include "InteractionScripts/LeverSystem.h"

// Sets default values for this component's properties
UTriangleCreator::UTriangleCreator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTriangleCreator::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UTriangleCreator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (TiedEdge1Lever && TiedEdge2Lever)
	{
		DeformEdges(TiedEdge1Lever->GetCurrentLeverValue(), TiedEdge2Lever->GetCurrentLeverValue());
	}
	if (!StartingComp || !Laser1Mesh || !Laser2Mesh || !Laser3Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("Somethings are nullptr"));
		ResetComponent();
		return;
	}

	EndingPoint1 = StartingComp->GetComponentLocation() + StartingComp->GetUpVector() * Line1Length;
	EndingPoint2 = StartingComp->GetComponentLocation() + StartingComp->GetRightVector() * Line2Length;

	Laser1Scale = FVector(0.05f, 0.05f, FVector::Distance(StartingComp->GetComponentLocation(), EndingPoint1) / InitialLaserScale);
	Laser1Mesh->SetWorldScale3D(Laser1Scale);
	Laser1Mesh->SetWorldRotation((EndingPoint1 - StartingComp->GetComponentLocation()).Rotation() - FRotator(90,0,0));

	Laser2Scale = FVector(0.05f, 0.05f, FVector::Distance(StartingComp->GetComponentLocation(), EndingPoint2) / InitialLaserScale);
	Laser2Mesh->SetWorldScale3D(Laser2Scale);
	Laser2Mesh->SetWorldRotation((EndingPoint2 - StartingComp->GetComponentLocation()).Rotation() - FRotator(90, 0, 0));

	Laser3Scale = FVector(0.05f, 0.05f, FVector::Distance(EndingPoint1, EndingPoint2) / InitialLaserScale);
	Laser3Mesh->SetWorldScale3D(Laser3Scale);
	Laser3Mesh->SetWorldRotation((EndingPoint1 - EndingPoint2).Rotation() - FRotator(90, 0, 0));
	Laser3Mesh->SetWorldLocation(EndingPoint2);

	FVector _Start = StartingComp->GetComponentLocation();
	//DrawDebugLine(GetWorld(), _Start, EndingPoint1, FColor::Red);
	//DrawDebugLine(GetWorld(), _Start, EndingPoint2, FColor::Blue);
	//DrawDebugLine(GetWorld(), EndingPoint1, EndingPoint2, FColor::Green);
}

void UTriangleCreator::SetInitialScales(USceneComponent* Start, USceneComponent* End)
{
	StartingComp = Start;
	InitialLaserScale = FVector::Distance(Start->GetComponentLocation(), End->GetComponentLocation());
	UE_LOG(LogTemp, Display, TEXT("InitialLaserScale: %f"), InitialLaserScale);
}

void UTriangleCreator::SetupComponent(USceneComponent* Start, UStaticMeshComponent* Laser1, UStaticMeshComponent* Laser2, UStaticMeshComponent* Laser3)
{
	StartingComp = Start;
	Laser1Mesh = Laser1;
	Laser2Mesh = Laser2;
	Laser3Mesh = Laser3;
}

void UTriangleCreator::DeformEdges(float Edge1Value, float Edge2Value)
{
	Line1Length += Edge1Value;
	Line2Length += Edge2Value;
	Line1Length = FMath::Clamp(Line1Length, -MaxLine1Length, MaxLine1Length);
	Line2Length = FMath::Clamp(Line2Length, -MaxLine2Length, MaxLine2Length);
}

