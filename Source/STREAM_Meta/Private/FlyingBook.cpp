// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingBook.h"
#include "Components/SplineComponent.h"

// Sets default values
AFlyingBook::AFlyingBook()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BookMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BookMesh"));
	BookPath = CreateDefaultSubobject<USplineComponent>(TEXT("BookPath"));

	BookMesh->SetupAttachment(RootComponent);
	BookPath->SetupAttachment(RootComponent);

	BookPath->SetClosedLoop(true);
}
// Called when the game starts or when spawned
void AFlyingBook::BeginPlay()
{
	Super::BeginPlay();
	BookMesh->PlayAnimation(BookFlyAnim, true);
}

// Called every frame
void AFlyingBook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveBook(DeltaTime);
}

void AFlyingBook::MoveBook(float DeltaSeconds)
{
	float _Upwards = BookMesh->GetForwardVector().Z;

	BookMesh->SetPlayRate((1 + (_Upwards)) * AnimationSpeedMultiplier);

	float _SlopedSpeed = NormalBookSpeed - (_Upwards * (NormalBookSpeed / 2));
	Distance += _SlopedSpeed * DeltaSeconds;
	if (Distance >= BookPath->GetSplineLength() - 5.f)
		Distance = 0.f;
	FTransform _BTransform = BookPath->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

	BookMesh->SetWorldLocation(_BTransform.GetLocation());
	BookMesh->SetWorldRotation(_BTransform.GetRotation());
}

