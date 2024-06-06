// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "PaintComponent.generated.h"

/**
 * 
 */
UCLASS()
class STREAM_META_API UPaintComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UPaintComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pen Informations", meta = (AllowPrivateAccess = "true"))
	/* Black = 0, Red = 1, Blue = 2, Green = 3 */
	int32 PenColorIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float PaintSize = 2.f;
};
