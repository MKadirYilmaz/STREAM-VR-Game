// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePen.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PaintingSystem/WritableActor.h"


// Sets default values
ABasePen::ABasePen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PenMesh"));
	PenTipZone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PenTipZone"));
	PenTipPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PenTipPoint"));

	RootComponent = PenMesh;
	PenTipZone->SetupAttachment(RootComponent);
	PenTipPoint->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void ABasePen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasePen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*
	FVector _Start = PenTipPoint->GetComponentLocation();
	FVector _End = _Start + PenTipPoint->GetForwardVector() * 200;
	DrawDebugLine(GetWorld(), _Start, _End, FColor::Red);

	
	bool _Hitted = GetWorld()->LineTraceSingleByChannel(PenHitResult, _Start, _End, ECC_Visibility);
	if (_Hitted)
	{
		AWritableActor* _Writable = Cast<AWritableActor>(PenHitResult.GetActor());
		if (_Writable)
		{
			Paint(PenHitResult);
		}
	}
	*/
	
}

