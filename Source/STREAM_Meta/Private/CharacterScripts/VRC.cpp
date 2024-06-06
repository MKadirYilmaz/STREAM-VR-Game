// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterScripts/VRC.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "InteractionScripts/HoldManager.h"
#include "InteractionScripts/LeverSystem.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "MotionControllerComponent.h"
#include "Components/CapsuleComponent.h"
#include "MathMiniGame/MathLaserMarker.h"
#include "TimeTravelZone.h"
#include "VehicleSystem/ToyPlane.h"


// VR CHARACTER SCRIPTS FOR VR CONTROLS. USE THIS CLASS!

// Sets default values
AVRC::AVRC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//CustomRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	//RootComponent = CustomRoot;

	//UCapsuleComponent* _Capsule = GetCapsuleComponent();

	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	CameraRoot->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraRoot);
	CameraCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CameraCollision"));
	CameraCollision->SetupAttachment(RootComponent);


	HandsRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HandsRoot"));
	HandsRoot->SetupAttachment(CameraRoot);
	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));
	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
	RightController->SetupAttachment(HandsRoot);
	LeftController->SetupAttachment(HandsRoot);

	RightHandWrist = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightHandWrist"));
	LeftHandWrist = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftHandWrist"));
	RightHandWrist->SetupAttachment(RightController);
	LeftHandWrist->SetupAttachment(LeftController);

	RightHandCollision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightHandCollision"));
	LeftHandCollision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftHandCollision"));
	RightHandCollision->SetupAttachment(RootComponent);
	LeftHandCollision->SetupAttachment(RootComponent);

	RightHandCollision->SetSimulatePhysics(true);
	LeftHandCollision->SetSimulatePhysics(true);

	RightHandPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RightHandPhysicsConstraint"));
	LeftHandPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LeftHandPhysicsConstraint"));
	RightHandPhysicsConstraint->SetupAttachment(RootComponent);
	LeftHandPhysicsConstraint->SetupAttachment(RootComponent);

	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	RightHandMesh->SetupAttachment(RightHandCollision);
	LeftHandMesh->SetupAttachment(LeftHandCollision);

	RightRaycastPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RightRaycastPoint"));
	LeftRaycastPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LeftRaycastPoint"));
	RightRaycastPoint->SetupAttachment(RightHandMesh);
	LeftRaycastPoint->SetupAttachment(LeftHandMesh);
	

	RightHolderPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RightHolderPhysicsConstraint"));
	LeftHolderPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LeftHolderPhysicsConstraint"));
	RightHolderPhysicsConstraint->SetupAttachment(RootComponent);
	LeftHolderPhysicsConstraint->SetupAttachment(RootComponent);

	RightHandPhysicsConstraint->SetConstrainedComponents(RightHandWrist, NAME_None, RightHandCollision, NAME_None);
	LeftHandPhysicsConstraint->SetConstrainedComponents(LeftHandWrist, NAME_None, LeftHandCollision, NAME_None);

	HoldManagerComp = CreateDefaultSubobject<UHoldManager>(TEXT("HoldManagerComp"));
}

// Called when the game starts or when spawned
void AVRC::BeginPlay()
{
	Super::BeginPlay();
	if (HoldManagerComp)
	{
		HoldManagerComp->SetPhysicsConstraints(LeftHolderPhysicsConstraint, RightHolderPhysicsConstraint);
	}
	MovementComp = GetCharacterMovement();
	
	TArray<AActor*> AllTimeTravelZones;
	UGameplayStatics::GetAllActorsOfClass(this, TimeTravelClass, AllTimeTravelZones);
	int32 i = 0;
	IsInsideTimeZones.Empty();
	for (AActor* _TimeZone : AllTimeTravelZones)
	{
		ATimeTravelZone* _Zone = Cast<ATimeTravelZone>(_TimeZone);
		_Zone->TimeZoneIndex = i;
		IsInsideTimeZones.Add(false);
		TimeZones.Add(_Zone);
		Cast<ATimeTravelZone>(_TimeZone)->TravelActivation.AddDynamic(this, &AVRC::SetInsideTimeZones);
		i++;
	}
}

// Called every frame
void AVRC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DeltaSeconds = DeltaTime;
	
	CameraCollision->SetWorldLocation(FVector(Camera->GetComponentLocation().X, Camera->GetComponentLocation().Y, GetActorLocation().Z));

	
	if (SmoothMovementActive) // Normal movement mode
	{
		FVector2D _MoveForward(0, MoveForwardValue);
		FVector2D _MoveRight(MoveRightValue, 0);
		float _Size = (_MoveForward + _MoveRight).Size();
		_Size = FMath::Clamp(_Size, 0, 1);

		CurrentMovementSpeed = CurrentMovementMode * _Size;
		FVector _Movement = (ForwardMovement + RightMovement).GetSafeNormal() * CurrentMovementSpeed * DeltaTime; // Hareket hizini hesapla
		AddMovementInput(_Movement);
	}
	else // Teleport movement mode
	{
		if (MoveForwardValue > 0.15f)
		{
			CurrentTeleportDist = TeleportationMaxDistance;

			FCollisionShape _Capsule = FCollisionShape::MakeCapsule(GetCapsuleComponent()->GetScaledCapsuleRadius(), GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

			FHitResult _TeleportHit;
			FVector _Start = LeftController->GetComponentLocation() + GetActorUpVector() * 100.f;
			FVector _End = _Start + LeftController->GetForwardVector() * TeleportationMaxDistance;
			FVector _MidPoint = _Start + LeftController->GetForwardVector() * (TeleportationMaxDistance / 2);
			_MidPoint += FVector(0, 0, 100.f);
			//DrawDebugLine(GetWorld(), _Start, _End, FColor::Red, false, -1.f, (uint8)0U, 1.f);
			//DrawDebugLine(GetWorld(), _MidPoint, _End, FColor::Yellow, false, -1.f, (uint8)0U, 2.f);
			bool _Hitted = GetWorld()->SweepSingleByChannel(_TeleportHit, _Start, _End, FQuat::Identity, ECC_Visibility, _Capsule); // Ilk duz tarama
			if (_Hitted)
			{
				SetTeleportRingVisibility(true);
				TeleportTargetLocation = _TeleportHit.Location;
				float _Slope = FVector::DotProduct(_TeleportHit.Normal, FVector(0, 0, 1.f));
				if (_Slope > 0.4f)
				{
					TeleportRingSetup(_TeleportHit.ImpactPoint, _TeleportHit.Normal, true);
					TeleportActive = true;
				}
				else // Egim cok dik veya tersse
				{
					TeleportRingSetup(_TeleportHit.ImpactPoint, _TeleportHit.Normal, false);
					TeleportActive = false;
				}
				//DrawDebugLine(GetWorld(), TeleportTargetLocation, TeleportTargetLocation + _TeleportHit.ImpactNormal * 20.f, FColor::Blue, false, -1.f, (uint8)0U, 1.f);
				//DrawDebugCapsule(GetWorld(), TeleportTargetLocation, GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), GetCapsuleComponent()->GetScaledCapsuleRadius(), FQuat::Identity, FColor::Green, false, -1.f, (uint8)0U, 1.f);
			}
			else
			{
				FVector _EndSecond = _End + (-GetActorUpVector() * (TeleportationMaxDistance / 2));
				//DrawDebugLine(GetWorld(), _End, _EndSecond, FColor::Blue, false, -1.f, (uint8)0U, 1.f);
				bool _HittedSecond = GetWorld()->SweepSingleByChannel(_TeleportHit, _End, _EndSecond, FQuat::Identity, ECC_Visibility, _Capsule); // Ikinci asagi tarama
				if (_HittedSecond) // Yukaridan asagiya dogru tarama
				{
					SetTeleportRingVisibility(true);
					TeleportTargetLocation = _TeleportHit.Location;
					float _Slope = FVector::DotProduct(_TeleportHit.Normal, FVector(0, 0, 1.f));
					if (_Slope > 0.4)
					{
						TeleportRingSetup(_TeleportHit.ImpactPoint, _TeleportHit.Normal, true);
						TeleportActive = true;
					}
					else // Egim cok dik veya tersse
					{
						TeleportRingSetup(_TeleportHit.ImpactPoint, _TeleportHit.Normal, false);
						TeleportActive = false;
					}
					//DrawDebugCapsule(GetWorld(), TeleportTargetLocation, GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), GetCapsuleComponent()->GetScaledCapsuleRadius(), FQuat::Identity, FColor::Green, false, -1.f, (uint8)0U, 1.f);
				}
				else
				{
					SetTeleportRingVisibility(false);
					TeleportActive = false;
				}
			}
		}
		if (MoveForwardValue < 0.15)
		{
			SetTeleportRingVisibility(false);
			if (TeleportActive)
			{
				//FHitResult _DoubleCheck;

				SetActorLocation(TeleportTargetLocation + GetActorUpVector() * 5.f); // Slightly higher
				TeleportActive = false;
				
			}
		}
	}

	if (TimeTravelling) // Time manipulation wrist system
	{
		RollDifference = RightHandCollision->GetRelativeRotation().Roll;
		RollDifference = FMath::Clamp(RollDifference, -60.f, 60.f);
		float _TargetTimeValue = CurrentTimeValue + (RollDifference / 100.f) * DeltaTime;
		CurrentTimeValue = FMath::FInterpTo(CurrentTimeValue, _TargetTimeValue, DeltaTime, 2.f);
		CurrentTimeValue = FMath::Clamp(CurrentTimeValue, 0.f, 1.f);
		TimeZones[CurrentTimeZoneIndex]->RunTimeTravel(CurrentTimeValue, DeltaTime);
	}
}

// Called to bind functionality to input
void AVRC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AVRC::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AVRC::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AVRC::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AVRC::LookRight);

	PlayerInputComponent->BindAxis(TEXT("Run / Accelerate"), this, &AVRC::Accelerate);

	PlayerInputComponent->BindAction(TEXT("InteractionRight"), IE_Pressed, this, &AVRC::RightHandInteraction);
	PlayerInputComponent->BindAction(TEXT("InteractionLeft"), IE_Pressed, this, &AVRC::LeftHandInteraction);

	PlayerInputComponent->BindAction(TEXT("InteractionRight"), IE_Released, this, &AVRC::ReleaseRightHand);
	PlayerInputComponent->BindAction(TEXT("InteractionLeft"), IE_Released, this, &AVRC::ReleaseLeftHand);

	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &AVRC::StartRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &AVRC::EndRun);

	PlayerInputComponent->BindAction(TEXT("ToggleMovementMode"), IE_Pressed, this, &AVRC::ToggleMovementMode);
	PlayerInputComponent->BindAction(TEXT("ToggleRotationMode"), IE_Pressed, this, &AVRC::ToggleRotationSystem);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AVRC::JumpingSystem);

	PlayerInputComponent->BindAction(TEXT("SecondaryInteraction"), IE_Pressed, this, &AVRC::SecondaryInteractionSystemRight);
	PlayerInputComponent->BindAction(TEXT("SecondaryInteractionLeft"), IE_Pressed, this, &AVRC::SecondaryInteractionSystemLeft);

	PlayerInputComponent->BindAction(TEXT("PlaneAbortFlight"), IE_Pressed, this, &AVRC::AbortPlaneFlight);

}

void AVRC::SetInsideTimeZones(bool IsInside, int32 TimeZoneIndex)
{
	IsInsideTimeZones[TimeZoneIndex] = IsInside;
}

void AVRC::MoveRight(float Value)
{
	if (MovementActive)
	{
		RightMovement = Camera->GetRightVector().GetSafeNormal2D() * Value;
	}
	else if (PlaneMovementActive)
	{
		ValueRight.Broadcast(Value);
	}
	MoveRightValue = Value;
}

void AVRC::MoveForward(float Value)
{
	if (MovementActive)
	{
		ForwardMovement = Camera->GetForwardVector().GetSafeNormal2D() * Value;
	}
	else if (PlaneMovementActive)
	{
		ValueForward.Broadcast(Value);
	}
	MoveForwardValue = Value;
}

void AVRC::LookUp(float Value)
{
	if (FreelookActive)
	{
		//AddControllerPitchInput(Value * MouseSensitivity * DeltaSeconds);
	}
	else if (PlaneMovementActive)
	{
		CameraRotationPitch.Broadcast(Value);
	}
	LookUpValue = Value;
}

void AVRC::LookRight(float Value)
{
	if (FreelookActive)
	{
		if (FMath::Abs(Value) > 0.25f)
		{
			FVector2D _Difference = FVector2D(Camera->GetComponentLocation().X, Camera->GetComponentLocation().Y) -
				FVector2D(RootComponent->GetComponentLocation().X, RootComponent->GetComponentLocation().Y);
			RootComponent->AddWorldOffset(FVector(_Difference.X, _Difference.Y, 0));
			CameraRoot->AddWorldOffset(-FVector(_Difference.X, _Difference.Y, 0));
		}
		if (SmoothRotationActive)
		{
			AddControllerYawInput(Value * MouseSensitivity * DeltaSeconds);
		}
		else // Sert donus sistemi aciksa
		{
			if (FMath::Abs(Value) < 0.35f) // Analog 0.35i gecince 45 derece dondur
			{
				HardRotationUsed = false;
			}
			else if (!HardRotationUsed)
			{
				GetWorldTimerManager().SetTimer(RotationDelayHandle, this, &AVRC::HardRotate, 0.05f);
				HardRotationUsed = true;
			}
		}
	}
	else if (PlaneMovementActive)
	{
		CameraRotationYaw.Broadcast(Value);
	}
	LookRightValue = Value;
}

void AVRC::Accelerate(float Value)
{
	if (PlaneMovementActive)
	{
		PlaneSpeedChange.Broadcast(Value);
	}
}

void AVRC::JumpingSystem()
{
	if (MovementComp)
	{
		if (!MovementComp->IsFalling())
		{
			Jump();
		}
		else if (!JSupperterUsed) // Yere degmeden once ziplama tusuna basilirsa 0.2 saniye sonra tekrar kontrol et
		{
			FTimerHandle KeySupporter;
			GetWorld()->GetTimerManager().SetTimer(KeySupporter, this, &AVRC::JumpingSystem, 0.2f, false);
			JSupperterUsed = true;
			return;
		}
		JSupperterUsed = false;
	}
}

void AVRC::InteractionSystem(UStaticMeshComponent* HandCollisionMesh, USceneComponent* RaycastPoint, UPhysicsConstraintComponent* HolderConstraint, int32 HandIndex)
{
	if (HandIndex == 1)
		GrabbingAnimBlueprint(true, true);
	else
		GrabbingAnimBlueprint(false, true);
	FHitResult InteractionHit;
	FCollisionShape _SweepShape = FCollisionShape::MakeSphere(InteractSweepRadius);
	FVector _Start = RaycastPoint->GetComponentLocation();
	FVector _End = _Start + RaycastPoint->GetForwardVector() * InteractionMaxDistance;
	//DrawDebugLine(GetWorld(), _Start, _End, FColor::Red);
	bool _Hitted = GetWorld()->SweepSingleByChannel(InteractionHit, _Start, _End, FQuat::Identity, ECC_GameTraceChannel1, _SweepShape);
	if (_Hitted)
	{
		if (InteractionHit.GetActor()->ActorHasTag(TEXT("HoldableObject")) && HoldManagerComp)
		{
			HoldManagerComp->HoldObjectWithPhysicsConstraint(InteractionHit, HandCollisionMesh, HolderConstraint, HandIndex);
			return;
		}
		else if (InteractionHit.GetActor()->ActorHasTag(TEXT("LeverSystem")))
		{
			if (HandIndex == -1)
			{
				LeftHoldedLever = Cast<ALeverSystem>(InteractionHit.GetActor());
				if(LeftHoldedLever)
					LeftHoldedLever->GrabLever(RaycastPoint);
			}
			else if (HandIndex == 1)
			{
				RightHoldedLever = Cast<ALeverSystem>(InteractionHit.GetActor());
				if (RightHoldedLever)
					RightHoldedLever->GrabLever(RaycastPoint);
			}	
		}
		else if (InteractionHit.GetActor()->ActorHasTag(TEXT("PlaneMiniGame")))
		{
			PlayingPlane = Cast<AToyPlane>(InteractionHit.GetActor());
			if (PlayingPlane)
			{
				PlayingPlane->StartFlying();
			}	
		}
		else
		{
			if (HandIndex == -1)
			{
				OILeft = true;
				OILeftHit = InteractionHit;
				OtherInteractionSystems(HandIndex, RaycastPoint, OILeftHit);
			}
			else if (HandIndex == 1)
			{
				OIRight = true;
				OIRightHit = InteractionHit;
				OtherInteractionSystems(HandIndex, RaycastPoint, OIRightHit);
			}
		}
	}
}

void AVRC::InteractableObjectInRange(USceneComponent* RaycastPoint, bool IsRightHand)
{
	FHitResult InteractionHit;
	FCollisionShape _SweepShape = FCollisionShape::MakeSphere(InteractSweepRadius);
	FVector _Start = RaycastPoint->GetComponentLocation();
	FVector _End = _Start + RaycastPoint->GetForwardVector() * InteractionMaxDistance;
	bool _Hitted = GetWorld()->SweepSingleByChannel(InteractionHit, _Start, _End, FQuat::Identity, ECC_GameTraceChannel1, _SweepShape);
	CanGrabAnimBlueprint(IsRightHand, _Hitted);
}

void AVRC::SecondaryInteractionSystemRight()
{
	if (HoldManagerComp->GetIsHoldingLaserMarkerRight())
	{
		AMathLaserMarker* _Laser = Cast<AMathLaserMarker>(HoldManagerComp->GetMathLaserMarkerRight());
		if (_Laser)
		{
			_Laser->LockOnBomb();
		}
	}
}

void AVRC::SecondaryInteractionSystemLeft()
{
	if (HoldManagerComp->GetIsHoldingLaserMarkerLeft())
	{
		AMathLaserMarker* _Laser = Cast<AMathLaserMarker>(HoldManagerComp->GetMathLaserMarkerLeft());
		if (_Laser)
		{
			_Laser->LockOnBomb();
		}
	}
}

void AVRC::RightHandInteraction()
{
	InteractionSystem(RightHandCollision, RightRaycastPoint, RightHolderPhysicsConstraint, 1);
	for (int32 i = 0; i < IsInsideTimeZones.Num(); i++)
	{
		if (IsInsideTimeZones[i] && !HoldManagerComp->GetIsHoldingRight())
		{
			TimeZones[i]->PrepareTimeTravel();
			CurrentTimeZoneIndex = i;
			TimeTravelling = true;
			StartRollValue = RightHandCollision->GetRelativeRotation().Roll;
			CurrentTimeValue = 1.f;
			break;
		}
	}
}

void AVRC::LeftHandInteraction()
{
	InteractionSystem(LeftHandCollision, LeftRaycastPoint, LeftHolderPhysicsConstraint, -1);
}

void AVRC::ReleaseRightHand()
{
	if (HoldManagerComp)
	{
		HoldManagerComp->ReleaseObjectWithPhysicsConstraint(1);
	}
	if (RightHoldedLever)
	{
		RightHoldedLever->ReleaseLever(RightRaycastPoint);
	}
	if (OIRight)
	{
		OtherReleaseSystems(1, RightRaycastPoint, OIRightHit);
		OIRight = false;
	}
	GrabbingAnimBlueprint(true, false);
	if (TimeTravelling)
	{
		TimeZones[CurrentTimeZoneIndex]->FinishTimeTravel();
		TimeTravelling = false;
	}
}

void AVRC::ReleaseLeftHand()
{
	if (HoldManagerComp)
	{
		HoldManagerComp->ReleaseObjectWithPhysicsConstraint(-1);
	}
	if (LeftHoldedLever)
	{
		LeftHoldedLever->ReleaseLever(LeftRaycastPoint);
	}
	if (OILeft)
	{
		OtherReleaseSystems(-1, LeftRaycastPoint, OILeftHit);
		OILeft = false;
	}
	GrabbingAnimBlueprint(false, false);
}

void AVRC::StartRun()
{
	CurrentMovementMode = RunMovementSpeed;
}

void AVRC::EndRun()
{
	CurrentMovementMode = MovementSpeed;
}

void AVRC::ToggleMovementMode()
{
	SmoothMovementActive = !SmoothMovementActive;
}

void AVRC::ToggleRotationSystem()
{
	SmoothRotationActive = !SmoothRotationActive;
}

void AVRC::HardRotate()
{
	AddControllerYawInput(LookRightValue * 45.f);
}

void AVRC::AbortPlaneFlight()
{
	if (PlayingPlane)
	{
		PlayingPlane->AbortPlaneFlight();
		PlayingPlane = nullptr;
	}
}


