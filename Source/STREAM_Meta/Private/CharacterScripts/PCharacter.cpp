// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterScripts/PCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "InteractionScripts/GrabManager.h"
#include "DrawDebugHelpers.h"
#include "InteractionScripts/HoldManager.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "MathMiniGame/MathBomb.h"
#include "MathMiniGame/MathGameSystem.h"
#include "GameManagers/MetaMode.h"
#include "VehicleSystem/ToyPlane.h"

// This class is for PC testing. Most of the game features does not supported in this version

// Sets default values
APCharacter::APCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(RootComponent);

	GrabHandlerRight = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("GrabHandlerRight"));
	GrabManager = CreateDefaultSubobject<UGrabManager>(TEXT("GrabManager"));
	HoldManager = CreateDefaultSubobject<UHoldManager>(TEXT("HoldManager"));

	RightInteractPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RightInteractPoint"));
	LeftInteractPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LeftInteractPoint"));

	RHNoPhysics = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RHNoPhysics"));
	RWristPhysics = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RWristPhysics"));
	RWristPhysics->SetupAttachment(RootComponent);
	RHNoPhysics->SetupAttachment(RootComponent);
	RHNoPhysics->SetSimulatePhysics(false);

	PCHandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PCHandMesh"));
	PCHandMesh->SetupAttachment(RootComponent);

	RightInteractPoint->SetupAttachment(PCHandMesh);
	LeftInteractPoint->SetupAttachment(RootComponent);

	//RWristPhysics->SetConstrainedComponents(RHNoPhysics, NAME_None, PCHandMesh, NAME_None);

	RightHolderConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RightHolderConstraint"));
	RightHolderConstraint->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APCharacter::BeginPlay()
{
	Super::BeginPlay();
	MovementComp = GetCharacterMovement();
}

// Called every frame
void APCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DeltaSeconds = DeltaTime;

	if (CurrentMovementMode == DefaultMovement)
	{
		FVector _Movement = (ForwardMovement + RightMovement).GetSafeNormal() * MovementComp->MaxWalkSpeed * DeltaTime; // Hareket hizini hesapla
		//CurrentMovement = FMath::VInterpTo(CurrentMovement, _Movement, DeltaTime, 15.f);
		AddMovementInput(_Movement);
	}
	
}

// Called to bind functionality to input
void APCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APCharacter::LookRight);

	PlayerInputComponent->BindAxis(TEXT("Run / Accelerate"), this, &APCharacter::SpeedUp);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APCharacter::JumpingSystem);
	PlayerInputComponent->BindAction(TEXT("InteractionLeft"), IE_Pressed, this, &APCharacter::InteractWithMouse);
	PlayerInputComponent->BindAction(TEXT("InteractionLeft"), IE_Released, this, &APCharacter::ReleaseInteraction);
	PlayerInputComponent->BindAction(TEXT("SlowWalk"), IE_Pressed, this, &APCharacter::ToggleSlowWalkMode);

	PlayerInputComponent->BindAction(TEXT("ToggleCarMode"), IE_Pressed, this, &APCharacter::ToggleCarMode);
	PlayerInputComponent->BindAction(TEXT("SecondaryInteraction"), IE_Pressed, this, &APCharacter::MathLockOnBomb);
}

void APCharacter::ToggleMovementSystem()
{
	if (MovementActive)
		MovementActive = false;
	else
		MovementActive = true;
}

void APCharacter::ChangeCustomMovementMode(int32 MovementModeIndex)
{
	switch (MovementModeIndex)
	{
	case 0:
		CurrentMovementMode = DefaultMovement;
		break;
	case 1:
		CurrentMovementMode = CarMovement;
		break;
	case 2:
		CurrentMovementMode = PlaneMovement;
		break;
	}
}

void APCharacter::LookUp(float Value)
{
	if (MovementActive)
	{
		if (AlternativeLooking)
		{
			FirstPersonCamera->AddRelativeRotation(FRotator(Value * MouseSensitivity * DeltaSeconds, 0, 0));
		}
		else
		{
			AddControllerPitchInput(Value * MouseSensitivity * DeltaSeconds);
		}
	}
	else if (CurrentMovementMode == PlaneMovement)
	{
		RotateCameraUp.Broadcast(Value);
	}
}

void APCharacter::LookRight(float Value)
{
	if (MovementActive)
	{
		if (AlternativeLooking)
		{
			FirstPersonCamera->AddRelativeRotation(FRotator(0, Value * MouseSensitivity * DeltaSeconds, 0));
		}
		else
		{
			AddControllerYawInput(Value * MouseSensitivity * DeltaSeconds);
		}
	}
	else if (CurrentMovementMode == PlaneMovement)
	{
		RotateCameraRight.Broadcast(Value);
	}
}

void APCharacter::ToggleSlowWalkMode()
{
	if (SlowWalkMode)
	{
		MovementComp->MaxWalkSpeed *= 3;
		SlowWalkMode = false;
	}
	else
	{
		MovementComp->MaxWalkSpeed /= 3;
		SlowWalkMode = true;
	}
}

void APCharacter::MoveForward(float Value)
{
	if (MovementActive)
	{
		if (!MovementComp)
			return;
		ForwardMovement = GetActorForwardVector() * Value;
	}
	ForwardAxis = Value;
}

void APCharacter::MoveRight(float Value)
{
	if (MovementActive)
	{
		if (!MovementComp)
			return;
		RightMovement = GetActorRightVector() * Value;
	}
	RightAxis = Value;
}

void APCharacter::JumpingSystem()
{
	if (!MovementComp->IsFalling())
	{
		Jump(); 
	}
	else if(!JSupperterUsed) // Yere degmeden once ziplama tusuna basilirsa 0.2 saniye sonra tekrar kontrol et
	{
		FTimerHandle KeySupporter;
		GetWorld()->GetTimerManager().SetTimer(KeySupporter, this, &APCharacter::JumpingSystem, 0.2f, false);
		JSupperterUsed = true;
		return;
	}
	JSupperterUsed = false;
}

void APCharacter::InteractWithMouse()
{
	InteractionSystem(FirstPersonCamera->GetComponentLocation(), FirstPersonCamera->GetForwardVector()); // Klasik mouse tuşu ile etkileşim
}

void APCharacter::ReleaseInteraction()
{
	switch(CurrentInteractionType)
	{
		case TelekineticInteraction:
		GrabManager->ReleaseObject(true); // PC'de tutan el, tek ve sag el olarak ayarlanmistir
		break;
		case HoldingInteraction:
		ReleaseAnObject();
		break;
		case MiniGamesStart:
		break;
		case WorldInteractions:
		break;
	}
}

bool APCharacter::InteractionSystem(const FVector& RaycastStart, const FVector& RaycastDirection)
{
	UE_LOG(LogTemp, Display, TEXT("PC Version of the Interaction System"));
	FCollisionShape _SweepShape = FCollisionShape::MakeSphere(InteractSweepRadius);
	FVector _SweepEnd = RaycastStart + RaycastDirection * InteractionMaxDist;
	DrawDebugLine(GetWorld(), RaycastStart, _SweepEnd, FColor::Red, false, 5.f);
	bool _Hitted = GetWorld()->SweepSingleByChannel(InteractResult, RaycastStart, _SweepEnd, FQuat::Identity, ECC_GameTraceChannel1, _SweepShape);
	if (_Hitted)
	{
		AActor* _HittedActor = InteractResult.GetActor();
		if (!_HittedActor || _HittedActor->Tags.Num() == 0)
			return false;
		
		FString _IntTagName = _HittedActor->Tags[0].ToString();
		if (_IntTagName == "PhysicsInteraction")
		{
			SetupInteractionSocketLocation(_HittedActor);
			GrabManager->SetDragVersion(false); // PC Versiyonunu kullan
			GrabManager->GrabObject(InteractResult.GetComponent(), InteractResult.ImpactPoint);
			CurrentInteractionType = TelekineticInteraction;
			return true;
		}
		else if(_IntTagName == "HoldableObject")
		{
			HoldAnObject();
			CurrentInteractionType = HoldingInteraction;
			return true;
		}
		else if (_IntTagName == "MiniGameStart")
		{
			CurrentInteractionType = MiniGamesStart;
			return true;
		}
		else if (_IntTagName == "VehicleInteraction")
		{
			CurrentInteractionType = WorldInteractions;
			return true;
		}
		return true;
	}
	else
	{
		return false;
	}
}

void APCharacter::HoldAnObject()
{
	//HoldManager->SetGrabbingPoint(PCConnectionMesh); // PC versiyonunda etkilesimler default olarak sag el olarak ayarlanmistir
	//HoldManager->HoldObject(InteractResult); 
	HoldManager->HoldObjectPhysicsConstraint(InteractResult, PCHandMesh);
}

void APCharacter::ReleaseAnObject()
{
	//HoldManager->ReleaseObject();
	HoldManager->ReleaseObjectPhysicsConstraint();
}

void APCharacter::ToggleCarMode()
{
	switch (CurrentMovementMode)
	{
	case DefaultMovement:
		CurrentMovementMode = CarMovement;
		UE_LOG(LogTemp, Warning, TEXT("Current Movement Mode: Car Movement"));
		break;
	case CarMovement:
		CurrentMovementMode = DefaultMovement;
		UE_LOG(LogTemp, Warning, TEXT("Current Movement Mode: Default Movement"));
		break;
	}
}

void APCharacter::SpeedUp(float Value)
{
	switch (CurrentMovementMode)
	{
	case DefaultMovement:
		break;
	case CarMovement:
		SpeedChangeEvent.Broadcast(Value);
		break;
	case PlaneMovement:
		SpeedChangeEvent.Broadcast(Value);
		break;
	}
}

void APCharacter::MathLockOnBomb()
{
	FVector _Start = FirstPersonCamera->GetComponentLocation();
	FVector _End = _Start + FirstPersonCamera->GetForwardVector() * 1000;
	DrawDebugLine(GetWorld(), _Start, _End, FColor::Red, false, 1.f);
	FHitResult _BombScan;
	bool _Hitted = GetWorld()->LineTraceSingleByChannel(_BombScan, _Start, _End, ECC_Visibility);
	if (_Hitted)
	{
		AMathBomb* _Bomb = Cast<AMathBomb>(_BombScan.GetActor());
		if (_Bomb)
		{
			Cast<AMetaMode>(UGameplayStatics::GetGameMode(this))->GetWorldMathGameSystem()->SetupScreenAndButtons(_Bomb->GetMainExpression(),
				_Bomb->GetChoice1(), _Bomb->GetChoice2(), _Bomb->GetAnswerIndex(), _Bomb);
		}
	}
}

void APCharacter::SetHolderPhysicsConstraints(UPhysicsConstraintComponent*& OutLeft, UPhysicsConstraintComponent*& OutRight)
{
	OutLeft = LeftHolderConstraint;
	OutRight = RightHolderConstraint;
}


