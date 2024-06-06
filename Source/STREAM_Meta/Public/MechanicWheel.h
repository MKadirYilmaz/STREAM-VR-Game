// All rights belong to Muhammed Kadir Yilmaz

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "MechanicWheel.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class STREAM_META_API UMechanicWheel : public UStaticMeshComponent
{
	GENERATED_BODY()
public:

	UMechanicWheel();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintImplementableEvent)
	void DirectEngine();
	UFUNCTION(BlueprintCallable)
	void GetEngineProperties(float EngineSpeed, float EnginePower);

	void SetFirstAdditionalChild(UMechanicWheel* NewChild);
protected:
	UPROPERTY(EditAnywhere)
	class ALeverSystem* TiedEngineSpeedLever;
	UPROPERTY(EditAnywhere)
	class ALeverSystem* TiedEnginePowerLever;

	UPROPERTY(EditAnywhere)
	float RotationSpeed = 0.f;
	UPROPERTY(EditAnywhere)
	int32 NumOfTeeth = 16;
	UPROPERTY(EditAnywhere)
	float WheelPower = 0.f;
	
	UPROPERTY(EditAnywhere)
	float EngineAccelerationSpeed = 0.75f;

	UPROPERTY(EditAnywhere)
	float EngineMaxForwardSpeed = 1000.f;
	UPROPERTY(EditAnywhere)
	float EngineMaxBackwardsSpeed = -500.f;

	UPROPERTY(EditAnywhere)
	float EngineMaxForwardTorque = 100.f;
	UPROPERTY(EditAnywhere)
	float EngineMaxBackwardsTorque = -50.f;


	UMechanicWheel* ChildWheel;

	TArray<UMechanicWheel*> AdditionalChilds; // These cannot be tied. They must be seperate

	UPROPERTY(EditAnywhere)
	bool StartingWheel = false;

	UPROPERTY(EditAnywhere)
	bool TiedWithChild = false;
public:
	UFUNCTION(BlueprintCallable)
	void SetChildWheelComp(UMechanicWheel* ChildComp) { ChildWheel = ChildComp; }

	UFUNCTION(BlueprintCallable)
	void SetAdditionalChildComps(TArray<UMechanicWheel*> Childs);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetWheelPower() const { return WheelPower; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetWheelSpeed() const { return RotationSpeed; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UMechanicWheel* GetChildGear() const { return ChildWheel; }
};
