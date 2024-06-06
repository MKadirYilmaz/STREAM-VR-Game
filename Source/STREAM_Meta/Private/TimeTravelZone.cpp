// All rights belong to Muhammed Kadir Yilmaz


#include "TimeTravelZone.h"
#include "Components/BoxComponent.h"
#include "TimeRewinder.h"
#include "GameFramework/Character.h"

// Sets default values
ATimeTravelZone::ATimeTravelZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TimeZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TimeZone"));

	RootComponent = TimeZone;

}

// Called when the game starts or when spawned
void ATimeTravelZone::BeginPlay()
{
	Super::BeginPlay();
	
	TimeZone->OnComponentBeginOverlap.AddDynamic(this, &ATimeTravelZone::OnBeginOverlap);
	TimeZone->OnComponentEndOverlap.AddDynamic(this, &ATimeTravelZone::OnEndOverlap);

	TimeRewinderComps.Empty();
	for (AActor* _TimeActor : TimeTravelActors)
	{
		UTimeRewinder* _Rewinder = _TimeActor->GetComponentByClass<UTimeRewinder>();
		if (_Rewinder)
		{
			TimeRewinderComps.Add(_Rewinder);
		}
	}
}

// Called every frame
void ATimeTravelZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATimeTravelZone::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ACharacter>(OtherActor))
	{
		TravelActivation.Broadcast(true, TimeZoneIndex);
		for (UTimeRewinder* _Rewinder : TimeRewinderComps)
		{
			_Rewinder->SetShouldTick(true);
		}
	}
}

void ATimeTravelZone::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ACharacter>(OtherActor))
	{
		TravelActivation.Broadcast(false, TimeZoneIndex);
		for (UTimeRewinder* _Rewinder : TimeRewinderComps)
		{
			_Rewinder->SetShouldTick(false);
		}
	}
}

void ATimeTravelZone::PrepareTimeTravel()
{
	for (UTimeRewinder* _Rewinder : TimeRewinderComps)
	{
		_Rewinder->PrepareRewind();
	}
}

void ATimeTravelZone::RunTimeTravel(float HandRotationValue, float DeltaTime)
{
	for (UTimeRewinder* _Rewinder : TimeRewinderComps)
	{
		_Rewinder->StartRewind(HandRotationValue, DeltaTime);
	}
}

void ATimeTravelZone::FinishTimeTravel()
{
	for (UTimeRewinder* _Rewinder : TimeRewinderComps)
	{
		_Rewinder->ExecuteRewind();
	}
}

