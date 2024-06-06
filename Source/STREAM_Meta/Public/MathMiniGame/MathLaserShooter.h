// All rights belong to Kadir Yilmaz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MathLaserShooter.generated.h"

UCLASS()
class STREAM_META_API AMathLaserShooter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMathLaserShooter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void FireLaser(int32 ButtonIndex);

	void SetFreeLaser();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LaserShooterMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LaserBeamMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AMathBomb* TargetBomb;

	bool ShouldTrace = false;

	FRotator DefaultRotation;
public:
	void SetTargetBomb(AMathBomb* BombActor) { TargetBomb = BombActor; }
	void SetShouldTrace(bool bShouldTrace) { ShouldTrace = bShouldTrace; }

	AMathBomb* GetLockedBomb() { return TargetBomb; }
};
