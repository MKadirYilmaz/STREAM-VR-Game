// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoDoor.h"
#include "Components/BoxComponent.h"

// Sets default values
AAutoDoor::AAutoDoor()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	ActivationZone = CreateDefaultSubobject<UBoxComponent>(TEXT("ActivationZone"));
	DoorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DoorMesh"));
	RootComponent = CustomRoot;
	ActivationZone->SetupAttachment(RootComponent);
	DoorMesh->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AAutoDoor::BeginPlay()
{
	Super::BeginPlay();

	ActivationZone->OnComponentBeginOverlap.AddDynamic(this, &AAutoDoor::OnActivationZoneOverlap);
	ActivationZone->OnComponentEndOverlap.AddDynamic(this, &AAutoDoor::OnActivationZoneEndOverlap);

}

// Called every frame
void AAutoDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAutoDoor::OnActivationZoneOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsDoorOpening)
	{
		UE_LOG(LogTemp, Display, TEXT("Auto doors are opening"));
		ActivateAutoDoor();
		IsDoorOpening = true;
	}
}

void AAutoDoor::OnActivationZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsDoorOpening)
	{
		UE_LOG(LogTemp, Display, TEXT("Auto doors are closing"));
		DeactivateAutoDoor();
		IsDoorOpening = false;
	}
}