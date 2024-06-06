// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleSystem/ToyPlane.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterScripts/PCharacter.h"
#include "Camera/CameraComponent.h"
#include "CharacterScripts/VRC.h"
#include "RacingSystem.h"
#include "GameManagers/MetaMode.h"

// Sets default values
AToyPlane::AToyPlane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BasePlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BasePlaneMesh"));
	PlaneCameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PlaneCameraRoot"));
	PlaneCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlaneCamera"));
	PlaneRestartPosition = CreateDefaultSubobject<USceneComponent>(TEXT("PlaneRestartPosition"));
	MotorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MotorMesh"));

	RootComponent = BasePlaneMesh;
	PlaneCameraRoot->SetupAttachment(RootComponent);
	PlaneCamera->SetupAttachment(PlaneCameraRoot);
	MotorMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AToyPlane::BeginPlay()
{
	Super::BeginPlay();
	StartingTransform = BasePlaneMesh->GetComponentTransform();
	Player = Cast<APCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	VR_Player = Cast<AVRC>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Player && IsPlaneActive)
	{
		Player->SpeedChangeEvent.AddDynamic(this, &AToyPlane::SpeedChange);
		Player->RotateCameraUp.AddDynamic(this, &AToyPlane::RotateCameraUp);
		Player->RotateCameraRight.AddDynamic(this, &AToyPlane::RotateCameraRight);
	}
	else if (VR_Player && IsPlaneActive)
	{
		VR_Player->CameraRotationYaw.AddDynamic(this, &AToyPlane::RotateCameraRight);
		VR_Player->CameraRotationPitch.AddDynamic(this, &AToyPlane::RotateCameraUp);
		VR_Player->PlaneSpeedChange.AddDynamic(this, &AToyPlane::SpeedChange);
		VR_Player->ValueForward.AddDynamic(this, &AToyPlane::RotateCameraRight);
		VR_Player->ValueRight.AddDynamic(this, &AToyPlane::RotateCameraRight);
	}
}

// Called every frame
void AToyPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!FlightStarted || !IsPlaneActive)
		return;

	RotateMotor(DeltaTime);
	if (Player) // PC kontrolleri
		RotatePlane(Player->GetMoveRightValue(), Player->GetMoveForwardValue(), DeltaTime);
	else if (VR_Player) // VR kontrolleri
	{
		RotatePlane(VR_Player->GetMoveRightValue(), VR_Player->GetMoveForwardValue(), DeltaTime);
		RotateHorizontally(VR_Player->GetLookRightValue(), DeltaTime);
	}
		
	CalculateDesiredDirection(DeltaTime);
	FVector _MovementVector = MovementDirection * CurrentSpeed * DeltaTime;

	//DrawDebugLine(GetWorld(), BasePlaneMesh->GetComponentLocation() + BasePlaneMesh->GetForwardVector() * 10, 
	//	(BasePlaneMesh->GetComponentLocation() + BasePlaneMesh->GetForwardVector() * 10) + DesiredDirection * 200, FColor::Red, false, -1.f, (uint8)0U, 2.f);

	//DrawDebugLine(GetWorld(), BasePlaneMesh->GetComponentLocation() + BasePlaneMesh->GetForwardVector() * 10, 
	//	(BasePlaneMesh->GetComponentLocation() + BasePlaneMesh->GetForwardVector() * 10) + MovementDirection * 200, FColor::Blue, false, -1.f, (uint8)0U, 1.f);

	if (DebugForceStop)
	{
		return;
	}
	FHitResult _HitResult;
	AddActorWorldOffset(_MovementVector, true, &_HitResult);
	if (_HitResult.GetActor() && !CollisionBroadcasted)
	{
		ACharacter* _IsCharacter = Cast<ACharacter>(_HitResult.GetActor());
		if (!_IsCharacter) // Carpilan sey karakterin kendisi degil ise
		{
			UE_LOG(LogTemp, Display, TEXT("Plane has crashed into an object"));
			//OnPlaneHit.Broadcast();
			AMetaMode* _GameMode = Cast<AMetaMode>(UGameplayStatics::GetGameMode(this));
			if (_GameMode)
			{
				ARacingSystem* _WorldRacingSystem = _GameMode->GetWorldRacingSystem();
				if (_WorldRacingSystem)
				{
					_WorldRacingSystem->CollisionRaceEnd();
					CollisionBroadcasted = true;
				}
			}

			
		}
	}
}

void AToyPlane::SwitchCamera(bool ToPlane)
{
	if (ToPlane)
	{
		TransitionToCamera(this, UGameplayStatics::GetPlayerController(this, 0));
	}
	else
	{
		if (!Player)
		{
			UE_LOG(LogTemp, Error, TEXT("Player is nullptr (ToyPlane / SwitchCamera)"));
			if (!VR_Player)
			{
				UE_LOG(LogTemp, Error, TEXT("VRPlayer is nullptr (ToyPlane / SwitchCamera)"));
				return;
			}
			else
			{
				TransitionToCamera(VR_Player, UGameplayStatics::GetPlayerController(this, 0));
			}
		}
		else
		{
			TransitionToCamera(Player, UGameplayStatics::GetPlayerController(this, 0));
		}
	}
}

void AToyPlane::StartFlying()
{
	if (Player)
	{
		Player->ToggleMovementSystem();
		Player->ChangeCustomMovementMode(2);
	}
	if (VR_Player)
	{
		VR_Player->SetPlaneMovement(true);
		VR_Player->SetFreelookActive(false);
		VR_Player->SetMovementActive(false);
	}
	AMetaMode* _GameMode = Cast<AMetaMode>(UGameplayStatics::GetGameMode(this));
	if (_GameMode)
	{
		ARacingSystem* _WorldRacingSystem = _GameMode->GetWorldRacingSystem();
		if (_WorldRacingSystem)
		{
			_WorldRacingSystem->StartRace(PlaneRouteIndex);
			_WorldRacingSystem->SetCurrentPlane(this);
			_WorldRacingSystem->SetCurrentRoute(PlaneRouteIndex);
		}
	}
	SoundSystem(true);
	RootComponent->SetWorldLocationAndRotation(PlaneRestartPosition->GetComponentLocation(), PlaneRestartPosition->GetComponentRotation());
	SwitchCamera(true);
	DesiredDirection = BasePlaneMesh->GetForwardVector();
	MovementDirection = BasePlaneMesh->GetForwardVector();
	CurrentSpeed = MaxSpeed / 3;
	FlightStarted = true;
}

void AToyPlane::StopFlying()
{
	if (Player)
	{
		Player->ToggleMovementSystem();
		Player->ChangeCustomMovementMode(0);
	}
	if (VR_Player)
	{
		VR_Player->SetPlaneMovement(false);
		VR_Player->SetFreelookActive(true);
		VR_Player->SetMovementActive(true);
	}
	
	SoundSystem(false);
	BasePlaneMesh->SetWorldTransform(StartingTransform);

	SwitchCamera(false);
	CurrentSpeed = 0;
	FlightStarted = false;
}

void AToyPlane::SpeedChange(float Value)
{
	CurrentSpeed += Value * UGameplayStatics::GetWorldDeltaSeconds(this) * AccelerationSpeed;
	CurrentSpeed = FMath::Clamp(CurrentSpeed, MaxSpeed / 3, MaxSpeed);
	//UE_LOG(LogTemp, Display, TEXT("CurrentSpeed = %f"), CurrentSpeed);
}

void AToyPlane::ReversePlaneRotation(bool IsReverse)
{
	PlaneRotationReverse = -PlaneRotationReverse;
}

void AToyPlane::ForceStopPlane()
{
	if (DebugForceStop)
	{
		DebugForceStop = false;
	}
	else
	{
		DebugForceStop = true;
	}
}

void AToyPlane::DebugMoveUpwards(float Value)
{
	SetActorLocation(GetActorLocation() + FVector(0, 0, Value * DebugMovingSpeed * UGameplayStatics::GetWorldDeltaSeconds(this)));
}

void AToyPlane::DebugMoveRight(float Value)
{
	SetActorLocation(GetActorLocation() + GetActorRightVector() * Value * DebugMovingSpeed * UGameplayStatics::GetWorldDeltaSeconds(this));
}

void AToyPlane::AbortPlaneFlight()
{
	AMetaMode* _GameMode = Cast<AMetaMode>(UGameplayStatics::GetGameMode(this));
	if (_GameMode)
	{
		ARacingSystem* _WorldRacingSystem = _GameMode->GetWorldRacingSystem();
		if (_WorldRacingSystem)
		{
			_WorldRacingSystem->EndRacePlaneTooFar();
		}
	}
}

void AToyPlane::RotateCameraUp(float Value)
{
	if (Player && CanLookUp)
	{
		PlaneCamera->AddLocalRotation(FRotator(-Value * Player->GetMouseSensitivity() * UGameplayStatics::GetWorldDeltaSeconds(this), 0, 0));
		PlaneCamera->SetRelativeRotation(FRotator(FMath::Clamp(PlaneCamera->GetRelativeRotation().Pitch, -70.f, 70.f), PlaneCamera->GetRelativeRotation().Yaw, 0));
		UE_LOG(LogTemp, Display, TEXT("Pitch: %f, Yaw: %f, Roll: %f"), PlaneCamera->GetRelativeRotation().Pitch, PlaneCamera->GetRelativeRotation().Yaw, PlaneCamera->GetRelativeRotation().Roll);
	}
	else if (VR_Player && CanLookUp)
	{
		PlaneCamera->AddLocalRotation(FRotator(-Value * VR_Player->GetMouseSensitivity() * UGameplayStatics::GetWorldDeltaSeconds(this), 0, 0));
		PlaneCamera->SetRelativeRotation(FRotator(FMath::Clamp(PlaneCamera->GetRelativeRotation().Pitch, -70.f, 70.f), PlaneCamera->GetRelativeRotation().Yaw, 0));
	}
}

void AToyPlane::RotateCameraRight(float Value)
{
	if (Player)
	{
		PlaneCamera->AddLocalRotation(FRotator(0, Value * Player->GetMouseSensitivity() * UGameplayStatics::GetWorldDeltaSeconds(this), 0));
	}
	else if (VR_Player)
	{
		PlaneCamera->AddLocalRotation(FRotator(0, Value * VR_Player->GetMouseSensitivity() * UGameplayStatics::GetWorldDeltaSeconds(this), 0));
	}
}

void AToyPlane::GetForwardValue(float Value)
{
	ForwardInput = Value;
}

void AToyPlane::GetRightValue(float Value)
{
	RightInput = Value;
}

void AToyPlane::RotateMotor(float DeltaSeconds)
{
	MotorMesh->AddLocalRotation(FRotator(0, 0, CurrentSpeed * 2 * DeltaSeconds));
}

float AToyPlane::GetCurrentTime() const
{
	AMetaMode* _GameMode = Cast<AMetaMode>(UGameplayStatics::GetGameMode(this));
	if (_GameMode)
	{
		ARacingSystem* _WorldRacingSystem = _GameMode->GetWorldRacingSystem();
		if (_WorldRacingSystem)
		{
			return _WorldRacingSystem->GetCurrentTime();
		}
	}
	return 0.f;

	
}

void AToyPlane::CalculateDesiredDirection(float DeltaSeconds)
{
	DesiredDirection = BasePlaneMesh->GetForwardVector();
	MovementDirection = FMath::VInterpTo(MovementDirection * 1000, DesiredDirection * 1000, DeltaSeconds, DirectionIntepSpeed).GetSafeNormal();
}

void AToyPlane::RotatePlane(float RightValue, float UpValue, float DeltaSeconds)
{
	RollRotationDelta = FMath::RInterpTo(RollRotationDelta, FRotator(0, 0, RightValue * DeltaSeconds * RollRotSpeed), DeltaSeconds, RollInterpSpeed);
	PitchRotationDelta = FMath::RInterpTo(PitchRotationDelta, FRotator(PlaneRotationReverse * UpValue * DeltaSeconds * PitchRotSpeed, 0, 0), DeltaSeconds, PitchInterpSpeed);
	BasePlaneMesh->AddLocalRotation(RollRotationDelta + PitchRotationDelta);
}

void AToyPlane::RotateHorizontally(float YawValue, float DeltaSeconds)
{
	YawRotationDelta = FMath::RInterpTo(YawRotationDelta, FRotator(0, YawValue * DeltaSeconds * YawRotSpeed, 0), DeltaSeconds, YawInterpSpeed);
	BasePlaneMesh->AddLocalRotation(YawRotationDelta);
}

void AToyPlane::ReturnToStartingPoint()
{
	BasePlaneMesh->SetWorldLocation(PlaneRestartPosition->GetComponentLocation());
	BasePlaneMesh->SetWorldRotation(PlaneRestartPosition->GetComponentRotation());
	CurrentSpeed = MaxSpeed / 2;
	MovementDirection = FVector::ZeroVector;
	DesiredDirection = FVector::ZeroVector;
}


