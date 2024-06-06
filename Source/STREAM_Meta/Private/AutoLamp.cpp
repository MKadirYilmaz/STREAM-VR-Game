// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoLamp.h"
#include "LightSource.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AAutoLamp::AAutoLamp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActivationBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ActivationBox"));

	RootComponent = ActivationBox;

}

// Called when the game starts or when spawned
void AAutoLamp::BeginPlay()
{
	Super::BeginPlay();

	for (ALightSource* LSource : LightSources)
	{
		LSource->SetStartCondition(false);
	}
	ActivationBox->OnComponentBeginOverlap.AddDynamic(this, &AAutoLamp::OpenLightSources);
	ActivationBox->OnComponentEndOverlap.AddDynamic(this, &AAutoLamp::CloseLightSources);
}

// Called every frame
void AAutoLamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (StartCountdown)
	{
		CurrentTime += DeltaTime;
		//UE_LOG(LogTemp, Display, TEXT("Seconds to closure: %f"), TimeToClose - CurrentTime);
		if (CurrentTime >= TimeToClose)
		{
			for (ALightSource* LSource : LightSources)
			{
				LSource->ChangeLightCondition(false);
			}
			StartCountdown = false;
			CanOpen = true;
			CurrentTime = 0.f;
		}
	}
}

void AAutoLamp::OpenLightSources(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ACharacter>(OtherActor) && CanOpen)
	{
		for (ALightSource* LSource : LightSources)
		{
			LSource->ChangeLightCondition(true);
		}
		StartCountdown = false;
		CanOpen = false;
		CurrentTime = 0.f;
	}
}

void AAutoLamp::CloseLightSources(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ACharacter>(OtherActor) && !CanOpen)
	{
		StartCountdown = true;
	}
}

