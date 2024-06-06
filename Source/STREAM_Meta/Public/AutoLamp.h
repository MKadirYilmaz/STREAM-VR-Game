// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AutoLamp.generated.h"

UCLASS()
class STREAM_META_API AAutoLamp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAutoLamp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeLightSwitch(bool IsOn);

	UFUNCTION()
	void OpenLightSources(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CloseLightSources(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


private:
	UPROPERTY(EditAnywhere)
	TArray<class ALightSource*> LightSources;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* ActivationBox;

	UPROPERTY(EditAnywhere)
	float TimeToClose = 5.f;

	TArray<AActor*> OverlappingActors;
	
	bool CanOpen = true;
	bool StartCountdown = false;
	float CurrentTime = 0.f;

};
