// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HoldManager.generated.h"


class UPhysicsConstraintComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STREAM_META_API UHoldManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHoldManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// PC Versiyonu icin ozellestirilmis tutma fonksiyonu
	void HoldObject(const FHitResult& InteractionResult);
	void HoldObject(const FHitResult& InteractionResult, bool IsRight);

	// PC Versiyonu icin ozellestirilmis birakma fonksiyonu
	void ReleaseObject();
	void ReleaseObject(bool IsRight);

	void HoldObjectPhysicsConstraint(const FHitResult& InteractionResult, UStaticMeshComponent* HandMesh);
	void ReleaseObjectPhysicsConstraint();

	void HoldObjectWithPhysicsConstraint(const FHitResult& InteractionResult, UStaticMeshComponent* HandCollision, UPhysicsConstraintComponent* HolderConstraint, int32 HandIndex);
	void ReleaseObjectWithPhysicsConstraint(int32 HandIndex);

	void SetPhysicsConstraints(UPhysicsConstraintComponent* LeftConstraint, UPhysicsConstraintComponent* RightConstraint);
private:
	class APhysicsObjects* RHTargetActor;
	UPrimitiveComponent* RHTargetComp;

	class APhysicsObjects* LHTargetActor;
	UPrimitiveComponent* LHTargetComp;

	USkeletalMeshComponent* RightGrabPoint;
	USkeletalMeshComponent* LeftGrabPoint;

	UPhysicsConstraintComponent* RightPhysicsConstraint;
	UPhysicsConstraintComponent* LeftPhysicsConstraint;

	UPrimitiveComponent* PCGrabPoint;

	UStaticMeshComponent* RightHandCollision;
	UStaticMeshComponent* LeftHandCollision;
	UPrimitiveComponent* RightHoldedComp;
	UPrimitiveComponent* LeftHoldedComp;
	
	bool IsHoldingRight = false;
	bool IsHoldingLeft = false;

	bool IsHoldingLaserMarkerR = false;
	bool IsHoldingLaserMarkerL = false;

	FRotator RHoldedCompRot;

	UPROPERTY(EditAnywhere)
	float ForceBreakConnectionDist = 100.f;

	AActor* MathLaserMarkerR;
	AActor* MathLaserMarkerL;
public:
	void SetGrabbingPoint(USkeletalMeshComponent* GrabbingPoint, bool IsRight);
	// PC versiyonundaki karakter icin asiri yuklenmis method
	void SetGrabbingPoint(UPrimitiveComponent* GrabbingPoint);

	bool GetIsHoldingRight() { return IsHoldingRight; }
	bool GetIsHoldingLeft() { return IsHoldingLeft; }

	bool GetIsHoldingLaserMarkerRight() { return IsHoldingLaserMarkerR; }
	bool GetIsHoldingLaserMarkerLeft() { return IsHoldingLaserMarkerL; }
		
	AActor* GetMathLaserMarkerRight() { return MathLaserMarkerR; }
	AActor* GetMathLaserMarkerLeft() { return MathLaserMarkerL; }
};
