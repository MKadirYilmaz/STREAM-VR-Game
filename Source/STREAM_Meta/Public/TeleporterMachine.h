// All rights belong to Kadir Yilmaz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleporterMachine.generated.h"

UCLASS()
class STREAM_META_API ATeleporterMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleporterMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void ExecDoorAnimation(bool DoorCondition);

	UFUNCTION(BlueprintCallable)
	void ReverseDoors();
	UFUNCTION(BlueprintCallable)
	void ExecuteTeleport();

private:
	UFUNCTION()
	void OnLeverActivated(bool IsPositive);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MachineMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* TeleportArea;

	UPROPERTY(EditAnywhere)
	FVector TeleportLocation1;
	UPROPERTY(EditAnywhere)
	FVector TeleportLocation2;
	UPROPERTY(EditAnywhere)
	FRotator TeleportRotation1;
	UPROPERTY(EditAnywhere)
	FRotator TeleportRotation2;

	bool TeleportFlipFlop = false;

	UPROPERTY(EditAnywhere)
	class ALeverSystem* TiedLever;

	TArray<FVector> ObjectLocations;


};
