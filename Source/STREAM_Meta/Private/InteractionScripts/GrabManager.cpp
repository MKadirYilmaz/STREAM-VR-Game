// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionScripts/GrabManager.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Camera/CameraComponent.h"
#include "CharacterScripts/PCharacter.h"
#include "CharacterScripts/VRCharacter.h"
#include "InteractionScripts/PhysicsObjects.h"

// Sets default values for this component's properties
UGrabManager::UGrabManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabManager::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<APCharacter>(GetOwner());
	AVRCharacter* _OwnerVRCharacter = Cast<AVRCharacter>(GetOwner());
	if(OwnerCharacter)
		GrabHandlerRight = OwnerCharacter->GetRightGrabHandle();
	if (_OwnerVRCharacter)
		GrabHandlerLeft = _OwnerVRCharacter->GetLeftGrabHandle();
	FirstPersonCamera = GetOwner()->GetComponentByClass<UCameraComponent>();
	
}


// Called every frame
void UGrabManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsGrabbing && bVRVersion) // Tutuyor ve VR ise
	{
		DragObjectVR();
	}
	else if (IsGrabbing && !bVRVersion) // Tutuyor ve PC ise
	{
		DragObjectPC();
	}
	// ...
}

void UGrabManager::GrabObject(UPrimitiveComponent* GrabbedComp, const FVector& GrabbedPoint)
{
	if (!GrabHandlerRight) // PC ve VR versiyonu icin gerekli olan PhysicsHandleComponent
	{
		UE_LOG(LogTemp, Error, TEXT("GrabHandler bulunamadi (GrabManager/GrabObject)"));
		return;
	}
	if (IsGrabbingRight || !bVRVersion) // Sag el ile tutuluyorsa veya PC versiyonu kullaniliyorsa
	{
		if (IsGrabbingLeft && LHTargetComp == GrabbedComp) // Ayni nesneyi halihazirda sol eliyle tutuyorsa
		{
			ReleaseObject(false); // Sagla tutmak icin solu birak
			GrabObject(GrabbedComp, GrabbedPoint);
		}
		RHTargetPO = Cast<APhysicsObjects>(GrabbedComp->GetOwner());
		if (!RHTargetPO)
		{
			UE_LOG(LogTemp, Error, TEXT("Grabbed Physics Object has not been found"));
			return;
		}
		RHTargetComp = GrabbedComp;
		RHTargetComp->SetEnableGravity(false);
		RHTargetPO->SetImpactPoint(GrabbedPoint);
		RHTargetComp->WakeAllRigidBodies();
		GrabHandlerRight->GrabComponentAtLocationWithRotation(RHTargetComp, NAME_None, GrabbedPoint, RHTargetComp->GetComponentRotation());
		IsGrabbingRight = true;
	}
	else // Sol el ile tutuluyorsa
	{
		if (!GrabHandlerLeft) // Sadece VR versiyonu icin gerekli olan PhysicsHandleComponent
			return;
		if (IsGrabbingRight && RHTargetComp == GrabbedComp) // Ayni nesneyi halihazirda sag eliyle tutuyorsa
		{
			ReleaseObject(true); // Solla tutmak icin sagi birak
			GrabObject(GrabbedComp, GrabbedPoint);
		}
		LHTargetPO = Cast<APhysicsObjects>(GrabbedComp->GetOwner());
		if (!RHTargetPO)
		{
			UE_LOG(LogTemp, Error, TEXT("Grabbed Physics Object has not been found"));
			return;
		}
		LHTargetComp = GrabbedComp;
		LHTargetComp->SetEnableGravity(false);
		LHTargetPO->SetImpactPoint(GrabbedPoint);
		LHTargetComp->WakeAllRigidBodies();
		GrabHandlerLeft->GrabComponentAtLocationWithRotation(LHTargetComp, NAME_None, GrabbedPoint, LHTargetComp->GetComponentRotation());
		IsGrabbingLeft = true;
	}
	IsGrabbing = true;
}

void UGrabManager::ReleaseObject(bool IsRight)
{
	if (!GrabHandlerRight)
	{
		UE_LOG(LogTemp, Error, TEXT("GrabHandler bulunamadi (GrabManager/ReleaseObject)"));
		return;
	}
	if (IsRight)
	{
		if (!RHTargetComp || !GrabHandlerRight)
			return;
		RHTargetComp->SetEnableGravity(true);
		GrabHandlerRight->ReleaseComponent();
		RHTargetComp = nullptr;
		RHTargetPO = nullptr;
		IsGrabbingRight = false;
	}
	else
	{
		if (!LHTargetComp || !GrabHandlerLeft)
			return;
		LHTargetComp->SetEnableGravity(true);
		GrabHandlerLeft->ReleaseComponent();
		LHTargetComp = nullptr;
		LHTargetPO = nullptr;
		IsGrabbingLeft = false;
	}
	RGrabberComp = nullptr;
	IsGrabbing = false;
}

void UGrabManager::DragObjectVR()
{
	if (!OwnerCharacter || !FirstPersonCamera)
	{
		UE_LOG(LogTemp, Error, TEXT("OwnerCharacter veya FirstPersonCamera bulunamadi (GrabManager/DragObject)"));
		return;
	}
	if (IsGrabbingRight && RGrabberComp && GrabHandlerRight && RHTargetComp)
	{
		DragCalculations(true);
	}
	if(IsGrabbingLeft && LGrabberComp && GrabHandlerLeft && LHTargetComp)
	{
		DragCalculations(false);
	}
}

void UGrabManager::DragObjectPC()
{
	if (!GrabHandlerRight || !FirstPersonCamera || !RHTargetComp)
	{
		UE_LOG(LogTemp, Error, TEXT("Drag Object PC nullptr"));
		return;
	}
	FVector _TargetLocation = FirstPersonCamera->GetComponentLocation() + FirstPersonCamera->GetForwardVector() * OwnerCharacter->GetGrabbingDistance();
	GrabHandlerRight->SetTargetLocationAndRotation(_TargetLocation, FirstPersonCamera->GetComponentRotation());
}

void UGrabManager::DragCalculations(bool IsRight)
{
	USceneComponent* _TempGrabComp = (IsRight) ? RGrabberComp : LGrabberComp; // Sag el veya sol eli ayarlar
	int32 _Multiplier = (IsRight) ? -1 : 1; // Sag ve sol elin avuc ici yonunu ayarlar
	FVector _TargetLoc = _TempGrabComp->GetComponentLocation() + ((_Multiplier * _TempGrabComp->GetRightVector()) * OwnerCharacter->GetGrabbingDistance());

	UPhysicsHandleComponent* _TempHandler = (IsRight) ? GrabHandlerRight : GrabHandlerLeft; // Sag veya sol physics handle component ayarlar

	UPrimitiveComponent* _TempTargetComp = (IsRight) ? RHTargetComp : LHTargetComp;
	FRotator _TargetRot = (_TempTargetComp->ComponentHasTag("SteeringWheel")) ? _TempTargetComp->GetComponentRotation() : _TempGrabComp->GetComponentRotation();

	_TempHandler->SetTargetLocationAndRotation(_TargetLoc, _TargetRot);

}

void UGrabManager::SetGrabberComponent(USceneComponent* TargetComponent, bool IsRight)
{
	(IsRight) ? RGrabberComp = TargetComponent : LGrabberComp = TargetComponent;
}

