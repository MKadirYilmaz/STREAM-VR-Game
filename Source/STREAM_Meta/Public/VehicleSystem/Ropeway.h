// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ropeway.generated.h"

UCLASS()
class STREAM_META_API ARopeway : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARopeway();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetupArrays();

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CustomRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USplineComponent* Ropeway;


	TArray<AActor*> AllTeleferics;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<float> AllTelefericTargetSpeeds;
	TArray<float> AllTelefericDistances;
	TArray<float> AllTelefericCurrentSpeeds;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SlowedSpeed = 100.f;

	UFUNCTION(BlueprintCallable)
	void SetTeleferics(TArray<AActor*> Teleferics) { AllTeleferics = Teleferics; }

	UFUNCTION(BlueprintCallable)
	int32 GetTelefericIndex(AActor* TelefericActor) const;

};
