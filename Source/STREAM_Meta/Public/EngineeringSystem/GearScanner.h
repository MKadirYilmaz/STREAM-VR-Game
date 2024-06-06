// All rights belong to Muhammed Kadir Yilmaz

#pragma once

#include "CoreMinimal.h"
#include "InteractionScripts/PhysicsObjects.h"
#include "GearScanner.generated.h"

/**
 * 
 */
UCLASS()
class STREAM_META_API AGearScanner : public APhysicsObjects
{
	GENERATED_BODY()

public:
	AGearScanner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetGearToScreen(UMechanicWheel* CurrentGear);

private:

	UPROPERTY(EditAnywhere)
	USceneComponent* LaserStart;
	UPROPERTY(EditAnywhere)
	USceneComponent* LaserEnd;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LaserMesh;

	UPROPERTY(EditAnywhere)
	float MaxLaserDistance = 500.f;

	class UMechanicWheel* ScannedGear;

	float InitialLaserScale;

	
};
