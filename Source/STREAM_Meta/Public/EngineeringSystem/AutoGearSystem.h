// All rights belong to Muhammed Kadir Yilmaz

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AutoGearSystem.generated.h"

class UMechanicWheel;
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STREAM_META_API UAutoGearSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAutoGearSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void UpdateEngineSpeed(float EngineSpeed);

	void SwitchToFirstGear();
	void SwitchToSecondGear();
	void SwitchToThirdGear();

	UFUNCTION(BlueprintImplementableEvent)
	void GearBoxToOne();
	UFUNCTION(BlueprintImplementableEvent)
	void GearBoxToTwo();
	UFUNCTION(BlueprintImplementableEvent)
	void GearBoxToThree();
private:

	UPROPERTY(EditAnywhere)
	float FirstToSecondDisplacement = 100.f;
	UPROPERTY(EditAnywhere)
	float SecondToThirdDisplacement = 100.f;

	UPROPERTY(EditAnywhere)
	float FirstGearMaxSpeed = 50.f;
	UPROPERTY(EditAnywhere)
	float SecondGearMaxSpeed = 100.f;
	UPROPERTY(EditAnywhere)
	float ThirdGearMaxSpeed = 150.f;
	UPROPERTY(EditAnywhere)
	float CurrentEngineSpeed = 0.f;
	UPROPERTY(VisibleAnywhere)
	float CurrentTireSpeed = 0.f;

	int32 CurrentGearIndex = 1;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMechanicWheel* EngineGear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMechanicWheel* TireGear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMechanicWheel* Gear1Connection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMechanicWheel* Gear2Connection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMechanicWheel* Gear3Connection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMechanicWheel* GearToWheel;

	UFUNCTION(BlueprintCallable)
	void SetCurrentEngineSpeed(float NewEngineSpeed) { CurrentEngineSpeed = NewEngineSpeed; }
	UFUNCTION(BlueprintCallable)
	void SetCurrentTireSpeed(float NewTireSpeed) { CurrentTireSpeed = NewTireSpeed; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentEngineSpeed() const { return CurrentEngineSpeed; }


};
