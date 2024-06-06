// Fill out your copyright notice in the Description page of Project Settings.


#include "LampSystem.h"
#include "Engine/Light.h"
#include "LightSource.h"

// Sets default values
ALampSystem::ALampSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EmptyRoot = CreateDefaultSubobject<UPrimitiveComponent>(TEXT("EmptyRoot"));
	LightSwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightSwitchMesh"));
	SwitchInteractZone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchInteractZone"));

	RootComponent = EmptyRoot;
	LightSwitchMesh->SetupAttachment(RootComponent);
	SwitchInteractZone->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALampSystem::BeginPlay()
{
	Super::BeginPlay();
	SwitchInteractZone->OnComponentBeginOverlap.AddDynamic(this, &ALampSystem::OnOverlapBegin);
	for (ALightSource* _CLight : LightSources)
	{
		_CLight->SetStartCondition(IsOpen);
	}
}

void ALampSystem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(IsOpen)
		LightSwitchMesh->AddRelativeRotation(FRotator(0, 0, 45));
	else
		LightSwitchMesh->AddRelativeRotation(FRotator(0, 0, -45));
	LightConditionChange();
}

// Called every frame
void ALampSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALampSystem::LightConditionChange()
{
	for (ALightSource* _CLight : LightSources)
	{
		//_CLight->ChangeLightCondition();
	}
}

