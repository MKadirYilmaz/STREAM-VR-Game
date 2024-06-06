// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicalButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonPressed, int32, PressedIndex);

UCLASS()
class STREAM_META_API APhysicalButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhysicalButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FOnButtonPressed ButtonPressed;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CustomRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ButtonMesh;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BottomMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ButtonTrigger;
	UPROPERTY(EditAnywhere)
	class UPhysicsConstraintComponent* ButtonConstraint;

	float DefaultDistance;
	UPROPERTY(EditAnywhere)
	float ButtonActiveRatio = 0.5f;
	UPROPERTY(EditAnywhere)
	float ButtonActivateDistance = 10.f;
	UPROPERTY(EditAnywhere)
	int32 ButtonIndex = 0;

	FString ButtonText;

	bool RepeatBroadcasting = false;
	bool Pressed = false;

public:
	void SetButtonText(FString NewText) { ButtonText = NewText; }

};
