// All rights belong to Muhammed Kadir Yilmaz

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeRewinder.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STREAM_META_API UTimeRewinder : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimeRewinder();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void PrepareRewind();
	UFUNCTION(BlueprintCallable)
	// Index Value Must Be Between 0 and 1
	void StartRewind(float IndexValue, float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void ExecuteRewind();

private:

	TArray<FTransform> Snapshots;
	TArray<FVector> LinearVelocities;
	TArray<FVector> RadialVelocities;

	bool SavedThisTick = false;
	bool ShouldTakeSnaps = true;
	bool IsRewinding = false;

	int32 ExecutedArrayIndex = 0;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsRewinding() const { return IsRewinding; }

	FORCEINLINE void SetShouldTick(bool ShouldTick) { ShouldTakeSnaps = ShouldTick; }


};
