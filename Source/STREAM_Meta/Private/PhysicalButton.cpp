// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicalButton.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
APhysicalButton::APhysicalButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	BottomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomMesh"));
	ButtonTrigger = CreateDefaultSubobject<USceneComponent>(TEXT("ButtonTrigger"));
	ButtonConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ButtonConstraint"));
	CustomRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomRoot"));

	RootComponent = CustomRoot;
	ButtonTrigger->SetupAttachment(RootComponent);
	ButtonMesh->SetupAttachment(RootComponent);
	BottomMesh->SetupAttachment(RootComponent);
	ButtonConstraint->SetupAttachment(RootComponent);

	ButtonMesh->SetSimulatePhysics(true);
	ButtonConstraint->SetConstrainedComponents(BottomMesh, NAME_None, ButtonMesh, NAME_None);

}

// Called when the game starts or when spawned
void APhysicalButton::BeginPlay()
{
	Super::BeginPlay();
	
	DefaultDistance = FVector::Distance(ButtonTrigger->GetComponentLocation(), ButtonMesh->GetComponentLocation());
}

// Called every frame
void APhysicalButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FVector::Distance(ButtonTrigger->GetComponentLocation(), ButtonMesh->GetComponentLocation()) / DefaultDistance < ButtonActiveRatio)
	{
		if(RepeatBroadcasting)
			ButtonPressed.Broadcast(ButtonIndex);
		else if (!Pressed)
		{
			ButtonPressed.Broadcast(ButtonIndex);
			Pressed = true;
		}
		//UE_LOG(LogTemp, Display, TEXT("Button Pressed"));
	}
	else
	{
		Pressed = false;
	}

}

