// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PController.generated.h"

/**
 * 
 */
UCLASS()
class STREAM_META_API APController : public APlayerController
{
	GENERATED_BODY()
public:
	APController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
