// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrabManager.generated.h"

class APhysicsObjects;
class APCharacter;
class UCameraComponent;
class UPhysicsHandleComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STREAM_META_API UGrabManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GrabObject(UPrimitiveComponent* GrabbedComp, const FVector& GrabbedPoint);
	void ReleaseObject(bool IsRight);
	void DragObjectVR();
	void DragObjectPC();
private:

	void DragCalculations(bool IsRight);
	UCameraComponent* FirstPersonCamera;
	APCharacter* OwnerCharacter;

	UPrimitiveComponent* RHTargetComp;
	APhysicsObjects* RHTargetPO;

	UPrimitiveComponent* LHTargetComp;
	APhysicsObjects* LHTargetPO;

	USceneComponent* RGrabberComp;
	USceneComponent* LGrabberComp;
	UPhysicsHandleComponent* GrabHandlerRight;
	UPhysicsHandleComponent* GrabHandlerLeft;

	bool IsGrabbing = false;

	bool IsGrabbingLeft, IsGrabbingRight;

	bool bVRVersion = true;

public:
	UPrimitiveComponent* GetGrabbingComponent() { return RHTargetComp; }
	void SetGrabberComponent(USceneComponent* TargetComponent, bool IsRight);
	void SetDragVersion(bool bVR) { bVRVersion = bVR; }
	

};
