// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PCharacter.h"
#include "VRCharacter.generated.h"

/**
 * 
 */
enum EInteractionHand
{
	RightHand,
	LeftHand,
	BothHands // -> Su anda kullanilamaz durumda
};


class UMotionControllerComponent;
class UPhysicsConstraintComponent;
class USpringArmComponent;
UCLASS()
class STREAM_META_API AVRCharacter : public APCharacter
{
	GENERATED_BODY()
public:
	AVRCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual bool InteractionSystem(const FVector& RaycastStart, const FVector& RaycastDirection) override;
	virtual void ReleaseInteraction() override;

private:
	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;

	virtual void HoldAnObject() override;
	virtual void ReleaseAnObject() override;

	void InteractWithRightHand();
	void StopInteractionRightHand();
	void InteractWithLeftHand();
	void StopInteractionLeftHand();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UMotionControllerComponent* RightMotionController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UMotionControllerComponent* LeftMotionController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* RightHandMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* LeftHandMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UPhysicsConstraintComponent* LWristPhysics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* RHandPMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* LHandPMesh;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* RWristMesh;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* LWristMesh;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* LHNoPhysics;

	UPROPERTY(EditAnywhere)
		USpringArmComponent* LeftSpringArm;
	UPROPERTY(EditAnywhere)
		USpringArmComponent* RightSpringArm;

		EInteractionHand CurrentInteractionHand;
		EInteractionHand CurrentReleasedHand;

	UPROPERTY(EditAnywhere)
		class UPhysicsHandleComponent* GrabHandlerLeft;

public:
	class UPhysicsHandleComponent* GetLeftGrabHandle() { return GrabHandlerLeft; }

};
