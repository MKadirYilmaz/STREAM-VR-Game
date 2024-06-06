// All rights belong to Kadir Yilmaz

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FibonacciCreator.generated.h"

class APhysicalButton;
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STREAM_META_API UFibonacciCreator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFibonacciCreator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void GetNextFibonacciNumber(int32 ButtonIndex);
	UFUNCTION(BlueprintCallable)
	void GetPrevFibonacciNumber(int32 ButtonIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void RefreshText();

private:

	UPROPERTY(EditAnywhere)
	APhysicalButton* TiedButtonNext;

	UPROPERTY(EditAnywhere)
	APhysicalButton* TiedButtonPrev;
	int64 Number1 = 1;
	int64 Number2 = 1;

	int64 PrevFibonacci = 0;

	int64 CurrentFibonacciNumber = Number2;
	int64 CurrentFibonacciIndex = 2;
public:
	UFUNCTION(BlueprintCallable)
	int64 GetCurrentFibonacci() const { return CurrentFibonacciNumber; }
	UFUNCTION(BlueprintCallable)
	int64 GetCurrentFibonacciIndex() const { return CurrentFibonacciIndex; }
};
