// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LampSystem.generated.h"

UCLASS()
class STREAM_META_API ALampSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALampSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void LightConditionChange();
private:

	UPROPERTY(EditAnywhere)
	TArray<class ALightSource*> LightSources;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LightSwitchMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SwitchInteractZone;

	UPrimitiveComponent* EmptyRoot;

	UPROPERTY(VisibleAnywhere, Category = "Light Switch Condition")
	bool IsOpen = false;

};
