// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightSource.generated.h"

UCLASS()
class STREAM_META_API ALightSource : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightSource();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ChangeLightCondition(bool bCondition);
	void SetStartCondition(bool StartCondition);

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* LightSourceMesh;

	UPROPERTY(EditAnywhere)
		UMaterialInstance* LightSourceMaterial;

	UPROPERTY(EditAnywhere)
		bool UsesSpotLight = false;
	UPROPERTY(EditAnywhere)
		bool LightStartCondition = false;
	UPROPERTY(EditAnywhere)
		float EmissivePower = 250.f;

	bool LCondition;
	float InitialIntensity;

	class ULocalLightComponent* LightComp;

	UMaterialInstanceDynamic* LSMDynamic;


};
