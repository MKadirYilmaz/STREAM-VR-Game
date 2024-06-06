// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleSystem/BaseCar.h"
#include "VehicleSystem/CarTireComponent.h"
#include "CharacterScripts/PCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseCar::ABaseCar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CarRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("CarRootComp"));
	BaseCarCollision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseCarCollision"));

	CarBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarBodyMesh"));

	FrontRight = CreateDefaultSubobject<UCarTireComponent>(TEXT("FrontRight"));
	FrontLeft = CreateDefaultSubobject<UCarTireComponent>(TEXT("FrontLeft"));
	BehindRight = CreateDefaultSubobject<UCarTireComponent>(TEXT("BehindRight"));
	BehindLeft = CreateDefaultSubobject<UCarTireComponent>(TEXT("BehindLeft"));

	SeatConnector = CreateDefaultSubobject<USceneComponent>(TEXT("SeatConnector"));

	RootComponent = CarRootComp;

	CarBodyMesh->SetupAttachment(RootComponent);
	BaseCarCollision->SetupAttachment(CarBodyMesh);

	FrontRight->SetupAttachment(CarBodyMesh);
	FrontLeft->SetupAttachment(CarBodyMesh);
	BehindRight->SetupAttachment(CarBodyMesh);
	BehindLeft->SetupAttachment(CarBodyMesh);

	SeatConnector->SetupAttachment(RootComponent);

	CarBodyMesh->SetSimulatePhysics(true);

}

// Called when the game starts or when spawned
void ABaseCar::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<APCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

// Called every frame
void ABaseCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator TireRotation = FrontRight->GetCurrentTireDirection().Rotation();

	FrontRight->SetWorldRotation(FRotator(FrontRight->GetComponentRotation().Pitch, TireRotation.Yaw, FrontRight->GetComponentRotation().Roll));
	FrontLeft->SetWorldRotation(FRotator(FrontLeft->GetComponentRotation().Pitch, TireRotation.Yaw, FrontLeft->GetComponentRotation().Roll));
	if (Player && Player->GetCustomMovementMode() == APCharacter::CarMovement)
	{
		FVector _Start = GetActorLocation();
		FVector _CarMovementDirection = GetCarSpeedVector(Player->GetMoveForwardValue(), DeltaTime);
		Player->SetActorLocation(SeatConnector->GetComponentLocation());
		FHitResult _CarHitResult;
		AddActorWorldOffset(_CarMovementDirection, true, &_CarHitResult);

		FRotator _NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + FrontRight->GetCurrentTireDirection()); // Normal durumdaki rotasyon hesaplamasi
		float _InterpMultiplier = FMath::Abs((_NewRotation - GetActorRotation()).GetNormalized().Yaw); // Direksiyon cevrildikce donus hizi artar (Duz gidise gore)

		if (CurrentSpeed < 0.f)
		{
			if (RightLeftValue == 1)
			{
				_NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + FVector::CrossProduct(-GetActorUpVector(),
					FrontRight->GetCurrentTireDirection().GetSafeNormal()));
			}
			else
			{
				_NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + FVector::CrossProduct(GetActorUpVector(),
					FrontRight->GetCurrentTireDirection().GetSafeNormal()));
			}
			DrawDebugLine(GetWorld(), _Start, _Start + _NewRotation.Vector() * 100.f, FColor::Blue);
		}
		if (GetVelocity().Size() > 1.f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("DeltaRotationYaw: %f"), (_NewRotation - GetActorRotation()).GetNormalized().Yaw);
			SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), _NewRotation, DeltaTime, GetVelocity().Size() *
				_InterpMultiplier / 400.f));
		}
	}
}

FVector ABaseCar::GetCarSpeedVector(float ForwardValue, float DeltaSeconds)
{

	if (ForwardValue > 0.1f) // Oyuncu ileri gitmeye calisiyorsa
	{
		MaxSpeed = ForwardMaxSpeed; // Ileri gitme max hizi

		if (CurrentSpeed < -0.1f) // Halihazirda geri gidiyorsa fren yaptir
		{
			CurrentCarAcceleration = BreakingAcceleration;
			RecalculateAccelerationForSlope(Breaking, CurrentCarAcceleration, MaxSpeed);
		}
		else // Geri gitmiyorsa hizlanma ivmesi ile hizlandir
		{
			CurrentCarAcceleration = ForwardAcceleration;
			RecalculateAccelerationForSlope(MovingForward, CurrentCarAcceleration, MaxSpeed);

		}
	}
	else if (ForwardValue < -0.1f) // Oyuncu geri gitmeye veya fren yapmaya calisiyorsa
	{
		MaxSpeed = -BackwardsMaxSpeed; // Geri gitme max hizi
		if (CurrentSpeed > 0.1f) // Halihazirda ileri gidiyorsa fren yaptir
		{
			CurrentCarAcceleration = BreakingAcceleration;
			RecalculateAccelerationForSlope(Breaking, CurrentCarAcceleration, MaxSpeed);
		}
		else // Ileri gitmiyorsa geri gitme ivmesi ile geriye dogru hizlandir
		{
			CurrentCarAcceleration = BackwardsAcceleration;
			RecalculateAccelerationForSlope(MovingBackwards, CurrentCarAcceleration, MaxSpeed);
		}
	}
	else // Hicbir tusa basilmadigi zaman
	{
		MaxSpeed = 0;
		CurrentCarAcceleration = StoppingAcceleration;
	}

	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, MaxSpeed, DeltaSeconds, CurrentCarAcceleration);
	return FrontRight->GetCurrentTireDirection().GetSafeNormal() * CurrentSpeed * DeltaSeconds;
}

FVector ABaseCar::GetSurfaceNormal()
{
	FHitResult SurfaceHitResult;
	FVector _Start = GetActorLocation();
	bool IsOnSurface = GetWorld()->LineTraceSingleByChannel(SurfaceHitResult, _Start, _Start + -GetActorUpVector() * 1000, ECC_Visibility);
	if (IsOnSurface && SurfaceHitResult.GetActor())
	{
		//UE_LOG(LogTemp, Warning, TEXT("SurfaceNormal: %s"), *SurfaceHitResult.ImpactNormal.ToString());
		return SurfaceHitResult.ImpactNormal;
	}
	else
		return FVector(0, 0, 0);
}

void ABaseCar::AlignCarWithSurface(const FVector& SurfaceNormal, float DeltaSeconds)
{
	FRotator _Rotation1 = FVector::CrossProduct(GetActorRightVector(), SurfaceNormal).Rotation();
	FRotator _Rotation2 = FVector::CrossProduct(GetActorForwardVector(), SurfaceNormal).Rotation();

	FRotator _NewRotation(_Rotation1.Pitch, GetActorRotation().Yaw, _Rotation2.Pitch);

	SetActorRotation(FMath::RInterpTo(GetActorRotation(), _NewRotation, DeltaSeconds, 2.5f));
}

void ABaseCar::RecalculateAccelerationForSlope(const ECarMoveStates& MovementState, float& OutCurrentAcceleration, float& OutCurrentMaxSpeed)
{
	switch (MovementState)
	{
	case MovingForward:
		SlopeRecalculations(1, OutCurrentAcceleration, OutCurrentMaxSpeed);
		break;
	case MovingBackwards:
		SlopeRecalculations(-1, OutCurrentAcceleration, OutCurrentMaxSpeed);
		break;

	case Breaking:
		if (Player->GetMoveForwardValue() > 0.1f)
		{
			SlopeRecalculations(1, OutCurrentAcceleration, OutCurrentMaxSpeed);
		}
		else if (Player->GetMoveForwardValue() < -0.1f)
		{
			SlopeRecalculations(-1, OutCurrentAcceleration, OutCurrentMaxSpeed);
		}
		break;
	default: // Duruyorsa
		break;
	}
}

void ABaseCar::SlopeRecalculations(int32 MovementDirection, float& OutCurrentAcceleration, float& OutCurrentMaxSpeed)
{
	float _DotProduct = FVector::DotProduct(FVector((MovementDirection)*GetActorForwardVector().X, (MovementDirection)*GetActorForwardVector().Y, 0), GetSurfaceNormal()); // 0 düz, 0,-1 arasi yokus yukari, 0,1 arasi yokus asagi
	float _AAdd = (OutCurrentAcceleration)*_DotProduct;
	float _SAdd = (OutCurrentMaxSpeed)*_DotProduct * 2;

	OutCurrentAcceleration += _AAdd;
	OutCurrentMaxSpeed += _SAdd;

	UE_LOG(LogTemp, Error, TEXT("NewMaxSpeed: %f, NewAcceleration: %f, Dot: %f"), OutCurrentMaxSpeed, OutCurrentAcceleration, _DotProduct);
}

