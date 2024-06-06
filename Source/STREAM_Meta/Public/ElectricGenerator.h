// All rights belong to Kadir Yilmaz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElectricGenerator.generated.h"

UCLASS()
class STREAM_META_API AElectricGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElectricGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetScreenText(float Percentage);

private:
	UFUNCTION()
	void OnOffSwitch(int32 ButtonIndex);

	void StoreVoltage();
	void ConsumeVoltageWithLight();

	UPROPERTY(EditAnywhere)
	USceneComponent* CustomRoot;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* FanMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* FanBody;

	UPROPERTY(EditAnywhere)
	class UPhysicsConstraintComponent* FanConstraint;

	UPROPERTY(EditAnywhere)
	class UPointLightComponent* PointLightComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ScreenMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* BatteryPercentageRenderer;

	UPROPERTY(EditAnywhere)
	float MaxStoredVoltage = 15000.f;

	UPROPERTY(EditAnywhere)
	class APhysicalButton* TiedButton;

	UPROPERTY(EditAnywhere)
	float VoltageProductionMultiplier = 10.f;
	UPROPERTY(EditAnywhere)
	float VoltageConsumeMultiplier = 1.f;

	float StoredVoltageAmount = 0.f;

	bool OnOffCondition = false;
	bool IsConsuming = false;

	FRotator LastTickRotation;


};
