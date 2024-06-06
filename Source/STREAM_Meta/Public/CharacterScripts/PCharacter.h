// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeSpeed, float, ChangeValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRotateCamera, float, RotateValue);

class UPhysicsConstraintComponent;
class UCharacterMovementComponent;
UCLASS()
class STREAM_META_API APCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	enum EInteractionType
	{
		TelekineticInteraction,
		HoldingInteraction,
		MiniGamesStart,
		WorldInteractions
	};
	enum ECustomMovementMode
	{
		DefaultMovement UMETA(DisplayName = "DefaultMovement"),
		CarMovement UMETA(DisplayName = "CarMovement"),
		PlaneMovement UMETA(DisplayName = "PlaneMovement")
	};

public:
	// Sets default values for this character's properties
	APCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
		void SetupInteractionSocketLocation(AActor* PhysicsObject);
	UFUNCTION(BlueprintCallable)
		void ToggleMovementSystem();

	UFUNCTION(BlueprintCallable)
		void ChangeCustomMovementMode(int32 MovementModeIndex);

	FChangeSpeed SpeedChangeEvent;
	FRotateCamera RotateCameraUp;
	FRotateCamera RotateCameraRight;
protected:
	UFUNCTION(BlueprintCallable)
		void LookUp(float Value);
	UFUNCTION(BlueprintCallable)
		void LookRight(float Value);

	void ToggleSlowWalkMode();
	bool SlowWalkMode = false;

	UFUNCTION(BlueprintCallable, meta = (Category = "Movement System"))
		virtual void MoveForward(float Value); // VR veya klavyeden gerekli inputu alır
	UFUNCTION(BlueprintCallable, meta = (Category = "Movement System"))
		virtual void MoveRight(float Value); // VR veya klavyeden gerekli inputu alır

	UFUNCTION(BlueprintCallable, meta = (Category = "Movement System"))
		void JumpingSystem(); // VR veya klavyeden gerekli inputu alır
	UFUNCTION(BlueprintCallable)
		void InteractWithMouse(); // Klavye/mousedan gerekli inputu alır
	UFUNCTION(BlueprintCallable)
		virtual void ReleaseInteraction();
	virtual bool InteractionSystem(const FVector& RaycastStart, const FVector& RaycastDirection); // Etkileşim sistemi için gerekli hesaplamalar ve uygulanış

	virtual void HoldAnObject();
	virtual void ReleaseAnObject();

	void ToggleCarMode();
	void SpeedUp(float Value);

	FVector CurrentMovement = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, Category = "Interaction System")
		float InteractionMaxDist = 200.f;
	UPROPERTY(EditAnywhere, Category = "Interaction System")
		float InteractSweepRadius = 10.f;
	UPROPERTY(EditAnywhere, Category = "Interaction System")
		float GrabbingDistance = 150.f;

	UPROPERTY(EditAnywhere)
		class UPhysicsHandleComponent* GrabHandlerRight;

	class UGrabManager* GrabManager;
	class UHoldManager* HoldManager;
	
	UPROPERTY(EditAnywhere)
		USceneComponent* RightInteractPoint;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* RHNoPhysics;



	UPROPERTY(EditAnywhere)
		USceneComponent* LeftInteractPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UPhysicsConstraintComponent* RWristPhysics;

	UPROPERTY(EditAnywhere)
		UPhysicsConstraintComponent* RightHolderConstraint;
	UPROPERTY(EditAnywhere)
		UPhysicsConstraintComponent* LeftHolderConstraint;

	class UCharacterMovementComponent* MovementComp;

	bool MovementActive = true;
	bool JSupperterUsed = false;
	FHitResult InteractResult;

	FVector ForwardMovement, RightMovement;
	float ForwardAxis, RightAxis;

	EInteractionType CurrentInteractionType;
private:
	void MathLockOnBomb();
	UPROPERTY(EditAnywhere)
		bool AlternativeLooking = false;
	UPROPERTY(EditAnywhere)
		float MouseSensitivity = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* PCHandMesh;
	
	float DeltaSeconds;

	
	ECustomMovementMode CurrentMovementMode = DefaultMovement;

public: // Getter, setter fonksiyonlari
	UFUNCTION(BlueprintCallable)
		FHitResult GetInteractionHitResult() const { return InteractResult; }
	float GetGrabbingDistance() { return GrabbingDistance; }
	
	ECustomMovementMode GetCustomMovementMode() { return CurrentMovementMode; }
	void SetCustomMovementMode(ECustomMovementMode NewMovementMode) { this->CurrentMovementMode = NewMovementMode; }

	float GetMoveForwardValue() const { return ForwardAxis; }
	float GetMoveRightValue() const { return RightAxis; }

	class UPhysicsHandleComponent* GetRightGrabHandle() { return GrabHandlerRight; }

	void SetHolderPhysicsConstraints(UPhysicsConstraintComponent*& OutLeft, UPhysicsConstraintComponent*& OutRight);
	UFUNCTION(BlueprintCallable)
	class UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCamera; }

	float GetMouseSensitivity() { return MouseSensitivity; }
	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetPhysicsHandMesh() { return PCHandMesh; }
};
