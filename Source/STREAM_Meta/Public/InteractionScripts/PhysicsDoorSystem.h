// All rights belong to Kadir Yilmaz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsDoorSystem.generated.h"

UCLASS()
class STREAM_META_API APhysicsDoorSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhysicsDoorSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SetDoorOpenCondition(bool bIsOpen);
	void SetDoorLockedCondition(bool bIsLocked);


	UFUNCTION(BlueprintCallable)
	void GrabHandle(USceneComponent* GrabComp);
	UFUNCTION(BlueprintCallable)
	void ReleaseHandle(USceneComponent* ReleaseComp);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CalculateHandleRotation(USceneComponent* Grabber, UStaticMeshComponent* HandleMesh);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CalculateDoorRotationForHand(USceneComponent* Grabber, UStaticMeshComponent* DoorStaticMesh);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ReturnDoorToOriginalPositon();
	UFUNCTION(BlueprintCallable)
	float GetRotationDifference() const;

	
	FRotator DoorCloseRotation;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorFrame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* DoorAndHandleTarget;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* DoorCloseZone;


	UPROPERTY(EditAnywhere)
	class UPhysicsConstraintComponent* FrameToDoorConstraint;

	UPROPERTY(EditAnywhere)
	class UPhysicsConstraintComponent* DoorToHandleConstraint;

	UPROPERTY(EditAnywhere, Category = "Door Properies")
	bool IsOpen = false;

	UPROPERTY(EditAnywhere, Category = "Door Properies")
	bool IsLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* GrabberComp;
	FVector GrabLocationDifference;

	bool ShouldTraceHand = false;

	bool IsDoorBeingPulled = false;
	bool IsDoorOpenPosition = false;

	bool IsGrabbing = false;

public:

	void SetIsOpen(bool bIsOpen) { IsOpen = bIsOpen; }
	void SetIsLocked(bool bIsLocked) { IsLocked = bIsLocked; }

	UFUNCTION(BlueprintCallable)
	void SetDoorBeingPulled(bool bIsBeingPulled) { IsDoorBeingPulled = bIsBeingPulled; }
	UFUNCTION(BlueprintCallable)
	void SetDoorOpenPosition(bool bDoorOpenPosition) { IsDoorOpenPosition = bDoorOpenPosition; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsLocked() const { return IsLocked; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsGrabbing() const { return IsGrabbing; }
};
