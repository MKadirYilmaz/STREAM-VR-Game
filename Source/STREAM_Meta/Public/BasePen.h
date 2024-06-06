// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePen.generated.h"

UCLASS()
class STREAM_META_API ABasePen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void Paint(const FHitResult& HitResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pen Details")
		float TipSize = 10.f;
	UPROPERTY(EditAnywhere, Category = "Pen Details")
		UStaticMeshComponent* PenMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pen Details")
		UTexture2D* PenTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* PenTipZone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* PenTipPoint;

	FHitResult PenHitResult;

};
