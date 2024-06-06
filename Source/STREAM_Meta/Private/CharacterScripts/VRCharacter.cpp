// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterScripts/VRCharacter.h"
#include "MotionControllerComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "InteractionScripts/GrabManager.h"
#include "Camera/CameraComponent.h"
#include "InteractionScripts/HoldManager.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/SpringArmComponent.h"


//
// THIS CLASS IS NOT WORKING! AN OLD TESTING VERSION OF VR CHARACTER. ABANDONED AND USELESS!!!
//


AVRCharacter::AVRCharacter()
{
	
	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));

	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));

	//RWristPhysics = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RWristPhysics"));
	LWristPhysics = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LWristPhysics"));

	RWristMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RWristMesh"));
	LWristMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LWristMesh"));
	RHandPMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RHandPMesh"));
	LHandPMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LHandPMesh"));

	LeftSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("LeftSpringArm"));
	RightSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("RightSpringArm"));

	GrabHandlerLeft = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("GrabHandlerLeft"));

	RightMotionController->MotionSource = FName("Right");
	LeftMotionController->MotionSource = FName("Left");

	RightMotionController->SetupAttachment(RootComponent);
	LeftMotionController->SetupAttachment(RootComponent);

	RightSpringArm->SetupAttachment(RightMotionController);
	LeftSpringArm->SetupAttachment(LeftMotionController);

	//RHandPMesh->SetupAttachment(RightSpringArm);
	//LHandPMesh->SetupAttachment(LeftSpringArm);
	RHandPMesh->SetSimulatePhysics(true);
	LHandPMesh->SetSimulatePhysics(true);
	RHandPMesh->SetCollisionObjectType(ECC_GameTraceChannel2); // ECC_GameTraceChannel2 = HandCollision
	LHandPMesh->SetCollisionObjectType(ECC_GameTraceChannel2); // ECC_GameTraceChannel2 = HandCollision

	RWristMesh->SetupAttachment(RightSpringArm);
	LWristMesh->SetupAttachment(LeftSpringArm);

	
	LHNoPhysics = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LHNoPhysics"));
	RHNoPhysics->SetupAttachment(RightMotionController); // RHNoPhysics PCharacter sinifi icinde olusturulmustu
	LHNoPhysics->SetupAttachment(LeftMotionController);
	LHNoPhysics->SetSimulatePhysics(false);
	LHNoPhysics->SetSimulatePhysics(false);

	// El collision component
	RHandPMesh->SetupAttachment(RootComponent);
	LHandPMesh->SetupAttachment(RootComponent);

	// El skeletal mesh
	RightHandMesh->SetupAttachment(RHandPMesh);
	LeftHandMesh->SetupAttachment(LHandPMesh);

	// Etkilesim ray cast isleminin yapilacagi nokta
	RightInteractPoint->SetupAttachment(RHandPMesh);
	LeftInteractPoint->SetupAttachment(LHandPMesh);

	// Physics Constraints
	RWristPhysics->SetupAttachment(RootComponent);
	RWristPhysics->ComponentName1.ComponentName = FName("RHNoPhysics");
	RWristPhysics->ComponentName2.ComponentName = FName("RHandPMesh");

	LWristPhysics->SetupAttachment(RootComponent);
	LWristPhysics->ComponentName1.ComponentName = FName("LHNoPhysics");
	LWristPhysics->ComponentName2.ComponentName = FName("LHandPMesh");
	
	LeftHolderConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LeftHolderConstraint"));
	LeftHolderConstraint->SetupAttachment(LHandPMesh);
	//RightHolderConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RightHolderConstraint"));
	RightHolderConstraint->SetupAttachment(RHandPMesh);


}

void AVRCharacter::BeginPlay()
{
	APCharacter::BeginPlay();
	RWristPhysics->ComponentName1.ComponentName = FName("RHNoPhysics");
	RWristPhysics->ComponentName2.ComponentName = FName("RHandPMesh");

	LWristPhysics->ComponentName1.ComponentName = FName("LHNoPhysics");
	LWristPhysics->ComponentName2.ComponentName = FName("LHandPMesh");
}

void AVRCharacter::Tick(float DeltaTime)
{
	APCharacter::Tick(DeltaTime);
	FVector RaycastStart = RightInteractPoint->GetComponentLocation();
	FVector RaycastDirection = -RightInteractPoint->GetRightVector();
	FCollisionShape _SweepShape = FCollisionShape::MakeSphere(InteractSweepRadius);
	FVector _SweepEnd = RaycastStart + RaycastDirection * 100;
	DrawDebugLine(GetWorld(), RaycastStart, _SweepEnd, FColor::Red);
	FHitResult _HitResult;
	bool _Hitted = GetWorld()->SweepSingleByChannel(_HitResult, RaycastStart, _SweepEnd, FQuat::Identity, ECC_GameTraceChannel1, _SweepShape);
	if (_Hitted)
	{
		FVector _V1 = RightHandMesh->GetForwardVector();
		FVector _V2 = _HitResult.Normal;
		DrawDebugLine(GetWorld(), RightHandMesh->GetComponentLocation(), RightHandMesh->GetComponentLocation() + FVector::CrossProduct(_V2, _V1) * 100.f, FColor::Green);
	}
}

void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->ClearActionBindings();

	PlayerInputComponent->BindAction(TEXT("InteractionRight"), IE_Pressed, this, &AVRCharacter::InteractWithRightHand);
	PlayerInputComponent->BindAction(TEXT("InteractionRight"), IE_Released, this, &AVRCharacter::StopInteractionRightHand);

	PlayerInputComponent->BindAction(TEXT("InteractionLeft"), IE_Pressed, this, &AVRCharacter::InteractWithLeftHand);
	PlayerInputComponent->BindAction(TEXT("InteractionLeft"), IE_Released, this, &AVRCharacter::StopInteractionLeftHand);
}

bool AVRCharacter::InteractionSystem(const FVector& RaycastStart, const FVector& RaycastDirection)
{
	UE_LOG(LogTemp, Display, TEXT("VR Version of the Interaction System"));
	FCollisionShape _SweepShape = FCollisionShape::MakeSphere(InteractSweepRadius);
	FVector _SweepEnd = RaycastStart + RaycastDirection * InteractionMaxDist;
	DrawDebugLine(GetWorld(), RaycastStart, _SweepEnd, FColor::Red, false, 5.f);
	bool _Hitted = GetWorld()->SweepSingleByChannel(InteractResult, RaycastStart, _SweepEnd, FQuat::Identity, ECC_GameTraceChannel1, _SweepShape); // Kolun rotasyonuna gore ileriye tarama yap
	if (_Hitted)
	{
		AActor* _HittedActor = InteractResult.GetActor();
		if (!_HittedActor || _HittedActor->Tags.Num() == 0)
			return false;

		FString _IntTagName = _HittedActor->Tags[0].ToString(); // !! Etkilesimin turunu yalnizca aktorun ilk tagi belirler !!
		if (_IntTagName == "PhysicsInteraction") // Uzaktan fizik temelli nesnelerle etkilesim
		{
			SetupInteractionSocketLocation(_HittedActor);
			GrabManager->GrabObject(InteractResult.GetComponent(), InteractResult.ImpactPoint);
			if (CurrentInteractionHand == RightHand) // Eger oyuncu sag eliyle tutarsa
			{
				GrabManager->SetGrabberComponent(RightInteractPoint, true);
			}
			else if (CurrentInteractionHand == LeftHand) // Eger oyuncu sol eliyle tutarsa
			{
				GrabManager->SetGrabberComponent(LeftInteractPoint, false);
			}
			CurrentInteractionType = TelekineticInteraction;
			return true;
		}
		else if (_IntTagName == "HoldableObject") // Kisa mesafeli fizik temelli nesnelere kenetlenerek etkilesim
		{
			HoldAnObject();
			CurrentInteractionType = HoldingInteraction;
			return true;
		}
		else if (_IntTagName == "MiniGameStart") // Mini oyun ve ozellikleri tetikleme sistemi
		{
			CurrentInteractionType = MiniGamesStart;
			return true;
		}
		return true;
	}
	else
	{
		return false;
	}
}

void AVRCharacter::ReleaseInteraction() // Etkilesim turune gore etkilesimi bitir
{
	switch (CurrentInteractionType)
	{
	case TelekineticInteraction:
		if (CurrentReleasedHand == RightHand)
			GrabManager->ReleaseObject(true);
		else if (CurrentReleasedHand == LeftHand)
			GrabManager->ReleaseObject(false);
		break;
	case HoldingInteraction:
		ReleaseAnObject();
		break;
	case MiniGamesStart:
		break;
	}
}

void AVRCharacter::MoveForward(float Value)
{
	if (!MovementComp)
		return;
	ForwardMovement = FirstPersonCamera->GetForwardVector().GetSafeNormal2D() * Value;
	ForwardAxis = Value;
}

void AVRCharacter::MoveRight(float Value)
{
	if (!MovementComp)
		return;
	RightMovement = FirstPersonCamera->GetRightVector().GetSafeNormal2D() * Value;
	RightAxis = Value;
}

void AVRCharacter::HoldAnObject()
{
	if (CurrentInteractionHand == RightHand) // Eger oyuncu sag eliyle tutarsa
	{
		HoldManager->SetGrabbingPoint(RightHandMesh, true);
		HoldManager->HoldObject(InteractResult, true);
	}
	else if (CurrentInteractionHand == LeftHand) // Eger oyuncu sol eliyle tutarsa
	{
		HoldManager->SetGrabbingPoint(LeftHandMesh, false);
		HoldManager->HoldObject(InteractResult, false);
	}
	
}

void AVRCharacter::ReleaseAnObject()
{
	if (CurrentReleasedHand == RightHand)
	{
		HoldManager->ReleaseObject(true);
	}
	else
	{
		HoldManager->ReleaseObject(false);
	}
}

void AVRCharacter::InteractWithRightHand()
{
	CurrentInteractionHand = RightHand;
	InteractionSystem(RightInteractPoint->GetComponentLocation(), -RightInteractPoint->GetRightVector());
}

void AVRCharacter::StopInteractionRightHand()
{
	CurrentReleasedHand = RightHand;
	ReleaseInteraction();
}

void AVRCharacter::InteractWithLeftHand()
{
	CurrentInteractionHand = LeftHand;
	InteractionSystem(LeftInteractPoint->GetComponentLocation(), LeftInteractPoint->GetRightVector());
}

void AVRCharacter::StopInteractionLeftHand()
{
	CurrentReleasedHand = LeftHand;
	ReleaseInteraction();
}
