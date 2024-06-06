// All rights belong to Muhammed Kadir Yilmaz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeTravelZone.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTimeTravelActivation, bool, Active, int32, Index);

class UTimeRewinder;
class ACharacter;
UCLASS()
class STREAM_META_API ATimeTravelZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimeTravelZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void PrepareTimeTravel();
	void RunTimeTravel(float HandRotationValue, float DeltaTime);
	void FinishTimeTravel();

	FTimeTravelActivation TravelActivation;

	int32 TimeZoneIndex;
private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* TimeZone;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> TimeTravelActors;

	TArray<UTimeRewinder*> TimeRewinderComps;

};
