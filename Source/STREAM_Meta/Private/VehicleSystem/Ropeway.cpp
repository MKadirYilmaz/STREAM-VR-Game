// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleSystem/Ropeway.h"
#include "Components/SplineComponent.h"

// Sets default values
ARopeway::ARopeway()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Ropeway = CreateDefaultSubobject<USplineComponent>(TEXT("Ropeway"));
	CustomRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomRoot"));

	RootComponent = CustomRoot;
	Ropeway->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ARopeway::BeginPlay()
{
	Super::BeginPlay();

	SetupArrays(); // Connection Points ve Cabins arraylerini ayarlar

	AllTelefericTargetSpeeds.SetNumZeroed(AllTeleferics.Num());
	AllTelefericDistances.SetNumZeroed(AllTeleferics.Num());
	AllTelefericCurrentSpeeds.SetNumZeroed(AllTeleferics.Num());
	float _Distance = 0;
	for (int32 i = 0; i < AllTeleferics.Num(); i++) 
	{
		FTransform _StartTransform = Ropeway->GetTransformAtDistanceAlongSpline(_Distance, ESplineCoordinateSpace::World);
		AllTelefericDistances[i] = _Distance;
		_Distance += Ropeway->GetSplineLength() / AllTeleferics.Num();

		AllTelefericCurrentSpeeds[i] = MaxSpeed;
		AllTelefericTargetSpeeds[i] = MaxSpeed;

		AllTeleferics[i]->SetActorLocationAndRotation(_StartTransform.GetLocation(), _StartTransform.GetRotation()); // Butun kabinler baslangic noktalarini alir
	}
}

// Called every frame
void ARopeway::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int32 i = 0; i < AllTeleferics.Num(); i++)
	{
		AllTelefericCurrentSpeeds[i] = FMath::FInterpTo(AllTelefericCurrentSpeeds[i], AllTelefericTargetSpeeds[i], DeltaTime, 2.5f);
		AllTelefericDistances[i] += AllTelefericCurrentSpeeds[i] * DeltaTime;
		if (AllTelefericDistances[i] > Ropeway->GetSplineLength() - 2.f)
		{
			AllTelefericDistances[i] = 0.f;
		}
		FTransform _CabinTransform = Ropeway->GetTransformAtDistanceAlongSpline(AllTelefericDistances[i], ESplineCoordinateSpace::World);

		AllTeleferics[i]->SetActorLocationAndRotation(_CabinTransform.GetLocation(), _CabinTransform.GetRotation());
	}
}

int32 ARopeway::GetTelefericIndex(AActor* TelefericActor) const
{
	int32 _Index;
	bool _Contain = AllTeleferics.Find(TelefericActor, _Index);
	if (_Contain)
	{
		return _Index;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Array does not contain this teleferic actor!"));
		return 0;
	}
}





