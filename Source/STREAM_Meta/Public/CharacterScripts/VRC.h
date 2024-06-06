// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRC.generated.h"

class UMotionControllerComponent;
class UPhysicsConstraintComponent;
class UCharacterMovementComponent;
class UHoldManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputValue, float, ChangeValue);

UCLASS()
class STREAM_META_API AVRC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRC();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Baska actorler tarafindan dinlenebilecek input degerleri
	FInputValue CameraRotationYaw;
	FInputValue CameraRotationPitch;
	FInputValue ValueForward;
	FInputValue ValueRight;
	FInputValue PlaneSpeedChange;

	UFUNCTION(BlueprintImplementableEvent)
	void OtherInteractionSystems(int32 HandIndex, USceneComponent* RaycastPoint, FHitResult OtherInteractionResult);
	UFUNCTION(BlueprintImplementableEvent)
	void OtherReleaseSystems(int32 HandIndex, USceneComponent* RaycastPoint, FHitResult OtherInteractionResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CanGrabAnimBlueprint(bool IsRightHand, bool CanGrab);
	UFUNCTION(BlueprintImplementableEvent)
	void GrabbingAnimBlueprint(bool IsRightHand, bool IsGrabbing);

	UFUNCTION(BlueprintImplementableEvent)
	void TeleportRingSetup(FVector RingLocation, FVector RingNormal, bool CanTeleport);
	UFUNCTION(BlueprintImplementableEvent)
	void SetTeleportRingVisibility(bool bShow);

	UFUNCTION()
	void SetInsideTimeZones(bool IsInside, int32 TimeZoneIndex);

private:
	void MoveRight(float Value);
	void MoveForward(float Value);
	void LookUp(float Value);
	void LookRight(float Value);

	void Accelerate(float Value);

	void JumpingSystem();

	void InteractionSystem(UStaticMeshComponent* HandCollisionMesh, USceneComponent* RaycastPoint, UPhysicsConstraintComponent* HolderConstraint, int32 HandIndex);
	void InteractableObjectInRange(USceneComponent* RaycastPoint, bool IsRightHand);
	void SecondaryInteractionSystemRight();
	void SecondaryInteractionSystemLeft();
	
	void RightHandInteraction();
	void LeftHandInteraction();
	bool OIRight, OILeft;
	FHitResult OIRightHit;
	FHitResult OILeftHit;

	void ReleaseRightHand();
	void ReleaseLeftHand();

	void StartRun();
	void EndRun();

	void ToggleMovementMode();
	void ToggleRotationSystem();
	void HardRotate();

	USceneComponent* CustomRoot;
	
	UPROPERTY(EditAnywhere)
	float MovementSpeed = 25.f;
	UPROPERTY(EditAnywhere)
	float RunMovementSpeed = 50.f;
	float CurrentMovementMode = MovementSpeed;
	float CurrentMovementSpeed = 0.f;

	UPROPERTY(EditAnywhere)
	float TeleportationMaxDistance = 150.f;
	float CurrentTeleportDist = 0.f;

	UPROPERTY(EditAnywhere)
	float MouseSensitivity = 50.f;
	UPROPERTY(EditAnywhere, Category = "Interaction System")
	float InteractionMaxDistance = 100.f;
	UPROPERTY(EditAnywhere, Category = "Interaction System")
	float InteractSweepRadius = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* CameraRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CameraCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* HandsRoot;
	UPROPERTY(EditAnywhere)
	UMotionControllerComponent* RightController;
	UPROPERTY(EditAnywhere)
	UMotionControllerComponent* LeftController;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RightHandWrist;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LeftHandWrist;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RightHandCollision;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LeftHandCollision;

	UPROPERTY(EditAnywhere)
	UPhysicsConstraintComponent* RightHandPhysicsConstraint;
	UPROPERTY(EditAnywhere)
	UPhysicsConstraintComponent* LeftHandPhysicsConstraint;

	UPROPERTY(EditAnywhere)
	UPhysicsConstraintComponent* RightHolderPhysicsConstraint;
	UPROPERTY(EditAnywhere)
	UPhysicsConstraintComponent* LeftHolderPhysicsConstraint;

	UPROPERTY(EditAnywhere)
	USceneComponent* RightRaycastPoint;
	UPROPERTY(EditAnywhere)
	USceneComponent* LeftRaycastPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* RightHandMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* LeftHandMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ATimeTravelZone> TimeTravelClass;

	UCharacterMovementComponent* MovementComp;
	UHoldManager* HoldManagerComp;

	FVector RightMovement, ForwardMovement;
	bool FreelookActive = true;
	bool MovementActive = true;
	bool PlaneMovementActive = true;

	bool JSupperterUsed = false;

	bool SmoothRotationActive = true;
	bool HardRotationUsed = false;
	FTimerHandle RotationDelayHandle;

	bool SmoothMovementActive = true;
	bool TeleportUsed = false;
	bool TeleportActive = false;
	FVector TeleportTargetLocation;

	float DeltaSeconds;
	float MoveRightValue, MoveForwardValue;
	float LookRightValue, LookUpValue;

	class ALeverSystem* RightHoldedLever;
	class ALeverSystem* LeftHoldedLever;

	class AToyPlane* PlayingPlane;
	void AbortPlaneFlight();

	// Time Travel Mechanic Zone
	TArray<class ATimeTravelZone*> TimeZones;
	TArray<bool> IsInsideTimeZones;
	int32 CurrentTimeZoneIndex;
	bool TimeTravelling = false;
	float StartRollValue;
	float RollDifference;
	float CurrentTimeValue = 1.f;

public:
	void SetPlaneMovement(bool NewPlaneMovement) { PlaneMovementActive = NewPlaneMovement; }
	void SetMovementActive(bool NewMovement) { MovementActive = NewMovement; }
	void SetFreelookActive(bool NewFreelook) { FreelookActive = NewFreelook; }


	float GetMouseSensitivity() const { return MouseSensitivity; }

	float GetMoveRightValue() const { return MoveRightValue; }
	float GetMoveForwardValue() const { return MoveForwardValue; }

	float GetLookUpValue() const { return LookUpValue; }
	float GetLookRightValue() const { return LookRightValue; }
};
