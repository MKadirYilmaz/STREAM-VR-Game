// All rights belong to Kadir Yilmaz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeverSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLeverActivation, bool, Positive);

UCLASS()
class STREAM_META_API ALeverSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeverSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FLeverActivation LeverActivated;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CalculateLeverRotation(USceneComponent* GrabberHand, UStaticMeshComponent* Handle);

	void GrabLever(USceneComponent* GrabbedComp);
	void ReleaseLever(USceneComponent* ReleasedComp);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ResetLeverRotation();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetLeverPitchValue(float Value);
private:

	USceneComponent* CustomRoot;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LeverBaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HandleMesh;
	UPROPERTY(EditAnywhere)
	ALeverSystem* ChildLever;

	//UPROPERTY(EditAnywhere)
	//class UPhysicsConstraintComponent* LeverPhysicsConstraint;

	float CurrentLeverValue;
	UPROPERTY(EditAnywhere)
	float LeverMaxValue = 0.9f;
	UPROPERTY(EditAnywhere)
	float LeverMinValue = -0.9f;

	UPROPERTY(EditAnywhere)
	float StartDeltaRotation = 0.f;

	UPROPERTY(EditAnywhere)
	bool StartActive = true;

	bool PositiveActivated, NegativeActivated;

	UPROPERTY(EditAnywhere)
	bool IsHolding = false;

	bool Holdable = true;

	USceneComponent* HandComp;
	FRotator InitialRotation;
public:
	UFUNCTION(BlueprintCallable)
	float GetCurrentLeverValue() const { return CurrentLeverValue; }

	UStaticMeshComponent* GetHandleMesh() const { return HandleMesh; }
	void SetIsHolding(bool bIsHolding) { IsHolding = bIsHolding; }

	void SetHoldable(bool bHoldable) { Holdable = bHoldable; }
	UFUNCTION(BlueprintCallable)
	void SetLeverValue(float NewLeverValue) { CurrentLeverValue = NewLeverValue; };

};
