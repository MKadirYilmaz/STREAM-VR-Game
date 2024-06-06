// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionScripts/HoldManager.h"
#include "CharacterScripts/PCharacter.h"
#include "InteractionScripts/PhysicsObjects.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values for this component's properties
UHoldManager::UHoldManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UHoldManager::BeginPlay()
{
	Super::BeginPlay();

	APCharacter* _Character = Cast<APCharacter>(GetOwner());
	if (_Character) _Character->SetHolderPhysicsConstraints(LeftPhysicsConstraint, RightPhysicsConstraint);
}


// Called every frame
void UHoldManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	/*
	if (IsHoldingRight)
	{
		if (FVector::Distance(RightHandCollision->GetComponentLocation(), RightHoldedComp->GetComponentLocation()) > ForceBreakConnectionDist)
		{
			ReleaseObjectWithPhysicsConstraint(1);
			UE_LOG(LogTemp, Warning, TEXT("Right hand connection forced to break"));
		}
	}
	if (IsHoldingLeft)
	{
		if (FVector::Distance(LeftHandCollision->GetComponentLocation(), LeftHoldedComp->GetComponentLocation()) > ForceBreakConnectionDist)
		{
			ReleaseObjectWithPhysicsConstraint(-1);
			UE_LOG(LogTemp, Warning, TEXT("Left hand connection forced to break"));
		}
	}
	*/
}

void UHoldManager::HoldObject(const FHitResult& InteractionResult)
{
	RHTargetActor = Cast<APhysicsObjects>(InteractionResult.GetActor());
	RHTargetComp = InteractionResult.GetComponent();

	if(!RHTargetActor || !RHTargetComp)
	{
		UE_LOG(LogTemp, Error, TEXT("TargetActor veya TargetComp nullptr! UHoldManager/HoldObject (PC Version)"));
		return;
	}
	RHTargetComp->SetSimulatePhysics(false);
	//RHTargetComp->SetEnableGravity(false);
	RHTargetActor->SetImpactPoint(InteractionResult.ImpactPoint);

	RHTargetActor->AttachTheObjectToComponent(PCGrabPoint);
}

void UHoldManager::HoldObject(const FHitResult& InteractionResult, bool IsRight)
{
	if (IsRight) // Oyuncu sag el ile mi tutmaya calisiyor
	{
		RHTargetActor = Cast<APhysicsObjects>(InteractionResult.GetActor());
		RHTargetComp = InteractionResult.GetComponent();
		if (!RHTargetActor || !RHTargetComp)
		{
			UE_LOG(LogTemp, Error, TEXT("TargetActor veya TargetComp nullptr! UHoldManager/HoldObject (RightHand)"));
			return;
		}
		if (IsHoldingLeft && RHTargetActor == LHTargetActor) // Sol elin halihazirda tuttugunu sag el tutmaya calisirsa
		{
			ReleaseObject(false); // Sol eli birak
			HoldObject(InteractionResult, IsRight); // Sag el ile tekrar tutmayi dene
		}
		RHTargetComp->SetSimulatePhysics(false);
		RHTargetActor->SetImpactPoint(InteractionResult.ImpactPoint);
		if (InteractionResult.GetActor()->ActorHasTag("Painter"))
		{
			RHTargetActor->AttachTheObjectToComponent(RightGrabPoint, FName("PenHoldingPoint"));
		}
		else
		{
			RHTargetActor->AttachTheObjectToComponent(RightGrabPoint);
		}

		IsHoldingRight = true;
	}
	else // Oyuncu sol el ile mi tutmaya calisiyor
	{
		LHTargetActor = Cast<APhysicsObjects>(InteractionResult.GetActor());
		LHTargetComp = InteractionResult.GetComponent();
		if (!LHTargetActor || !LHTargetComp)
		{
			UE_LOG(LogTemp, Error, TEXT("TargetActor veya TargetComp nullptr! UHoldManager/HoldObject (LeftHand)"));
			return;
		}
		if (IsHoldingRight && RHTargetActor == LHTargetActor) // Sag elin halihazirda tuttugunu sol el tutmaya calisirsa
		{
			ReleaseObject(true); // Sag eli birak
			HoldObject(InteractionResult, IsRight); // Sol el ile tekrar tutmayi dene
		}
		LHTargetComp->SetSimulatePhysics(false);
		LHTargetActor->SetImpactPoint(InteractionResult.ImpactPoint);
		if (InteractionResult.GetActor()->ActorHasTag("Painter"))
		{
			LHTargetActor->AttachTheObjectToComponent(LeftGrabPoint, FName("PenHoldingPoint"));
		}
		else
		{
			LHTargetActor->AttachTheObjectToComponent(LeftGrabPoint);
		}
		IsHoldingLeft = true;
	}
}

void UHoldManager::ReleaseObject() // PC versiyonu
{
	RHTargetComp->SetSimulatePhysics(true);
	//RHTargetComp->SetEnableGravity(true);
	RHTargetActor->BreakAttachmentWithComponent();
}

void UHoldManager::ReleaseObject(bool IsRight)
{

	if (IsRight && IsHoldingRight) // Oyuncu sag elindekini mi birakti
	{
		if (!RHTargetActor || !RHTargetComp) // Double check
		{
			UE_LOG(LogTemp, Error, TEXT("TargetActor veya TargetComp nullptr! UHoldManager/ReleaseObject (RightHand)"));
			return;
		}
		RHTargetComp->SetSimulatePhysics(true);
		RHTargetActor->BreakAttachmentWithComponent();
		RHTargetActor = nullptr;
		RHTargetComp = nullptr;
		RightGrabPoint = nullptr;
		IsHoldingRight = false;
	}
	else if(!IsRight && IsHoldingLeft) // Oyuncu sol elindekini mi birakti
	{
		if (!LHTargetActor || !LHTargetComp) // Double check
		{
			UE_LOG(LogTemp, Error, TEXT("TargetActor veya TargetComp nullptr! UHoldManager/ReleaseObject (LeftHand)"));
			return;
		}
		LHTargetComp->SetSimulatePhysics(true);
		LHTargetActor->BreakAttachmentWithComponent();
		LHTargetComp = nullptr;
		LHTargetActor = nullptr;
		LeftGrabPoint = nullptr;
		IsHoldingLeft = false;
	}
	
}

void UHoldManager::HoldObjectPhysicsConstraint(const FHitResult& InteractionResult, UStaticMeshComponent* HandMesh)
{
	RHTargetActor = Cast<APhysicsObjects>(InteractionResult.GetActor());
	RHTargetComp = InteractionResult.GetComponent();

	if (!RHTargetComp || !RightPhysicsConstraint)
	{
		UE_LOG(LogTemp, Error, TEXT("TargetComp or PhysicsConstraint nullptr! UHoldManager/HoldObjectPhysicsConstraint"));
		return;
	}
	//RHTargetComp->SetSimulatePhysics(true); // Tutulan nesnenin fizik simulasyonunun acik oldugundan emin olmak icin
	//RightPhysicsConstraint->ConstraintActor2 = RHTargetActor;
	//RightPhysicsConstraint->ComponentName2.ComponentName = FName(RHTargetComp->GetName());
	if (RightPhysicsConstraint)
	{
		RHoldedCompRot = RHTargetComp->GetComponentRotation();
		RightPhysicsConstraint->SetConstrainedComponents(HandMesh, NAME_None, RHTargetComp, NAME_None);
		//RightPhysicsConstraint->SetAngularOrientationTarget(_PrevRot);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("RightPhysicsConstraint is nullptr! (HoldManager/HoldObjectPhysicsConstraint)"));
	}
}

void UHoldManager::ReleaseObjectPhysicsConstraint()
{
	if (!RHTargetComp || !RightPhysicsConstraint)
	{
		UE_LOG(LogTemp, Error, TEXT("TargetComp or PhysicsConstraint nullptr! UHoldManager/HoldObjectPhysicsConstraint"));
		return;
	}
	RightPhysicsConstraint->BreakConstraint();

	RHTargetActor = nullptr;
	RHTargetComp = nullptr;

}
// Use this function to hold an object
void UHoldManager::HoldObjectWithPhysicsConstraint(const FHitResult& InteractionResult, UStaticMeshComponent* HandCollision, UPhysicsConstraintComponent* HolderConstraint, int32 HandIndex)
{
	if (HolderConstraint)
	{
		InteractionResult.GetComponent()->SetSimulatePhysics(true);
		HolderConstraint->SetConstrainedComponents(HandCollision, NAME_None, InteractionResult.GetComponent(), NAME_None);
		if (HandIndex < 0) // Sol el
		{
			LeftHoldedComp = InteractionResult.GetComponent();
			LeftHandCollision = HandCollision;
			IsHoldingLeft = true;
			if (InteractionResult.GetActor()->ActorHasTag("MathLaserMarker"))
			{
				MathLaserMarkerL = InteractionResult.GetActor();
				IsHoldingLaserMarkerL = true;
			}
		}
		else if (HandIndex > 0) // Sag el
		{
			RightHoldedComp = InteractionResult.GetComponent();
			RightHandCollision = HandCollision;
			IsHoldingRight = true;
			if (InteractionResult.GetActor()->ActorHasTag("MathLaserMarker"))
			{
				MathLaserMarkerR = InteractionResult.GetActor();
				IsHoldingLaserMarkerR = true;
			}
		}
	}
}
// Use this function to release an object
void UHoldManager::ReleaseObjectWithPhysicsConstraint(int32 HandIndex)
{
	if (HandIndex < 0) // Sol el
	{
		if (LeftPhysicsConstraint)
		{
			LeftPhysicsConstraint->BreakConstraint();
			IsHoldingLeft = false;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("LeftPhysicsConstraint is nullptr"));
		}
		if (IsHoldingLaserMarkerL)
		{
			MathLaserMarkerL = nullptr;
			IsHoldingLaserMarkerL = false;
		}
	}
	else if (HandIndex > 0) // Sag el
	{
		if (RightPhysicsConstraint)
		{
			RightPhysicsConstraint->BreakConstraint();
			IsHoldingRight = false;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("RightPhysicsConstraint is nullptr"));
		}
		if (IsHoldingLaserMarkerR)
		{
			MathLaserMarkerR = nullptr;
			IsHoldingLaserMarkerR = false;
		}
	}
}

void UHoldManager::SetPhysicsConstraints(UPhysicsConstraintComponent* LeftConstraint, UPhysicsConstraintComponent* RightConstraint)
{
	LeftPhysicsConstraint = LeftConstraint;
	RightPhysicsConstraint = RightConstraint;
}

void UHoldManager::SetGrabbingPoint(USkeletalMeshComponent* GrabbingPoint, bool IsRight)
{
	if (IsRight)
		RightGrabPoint = GrabbingPoint;
	else
		LeftGrabPoint = GrabbingPoint;
}

void UHoldManager::SetGrabbingPoint(UPrimitiveComponent* GrabbingPoint)
{
	PCGrabPoint = GrabbingPoint;
}
