// All rights belong to Kadir Yilmaz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BowlingLabuts.generated.h"

UCLASS()
class STREAM_META_API ABowlingLabuts : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABowlingLabuts();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ResetLabutTransforms(int32 ButtonIndex);

	void SetLabutSimulation(bool bPhysicsEnabled);
	void EnableLabutSimulation();
	bool CheckAllLabutsShouldReturn();
private:

	UPROPERTY(EditAnywhere)
	USceneComponent* LabutsRoot;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Labut1;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Labut2;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Labut3;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Labut4;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Labut5;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Labut6;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Labut7;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Labut8;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Labut9;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Labut10;
	
	UPROPERTY(EditAnywhere)
	class APhysicalButton* TiedButton;

	bool ShouldReturn = false;
	TArray<UStaticMeshComponent*> Labuts;
	TArray<FTransform> LabutDefaultTransforms;
	TArray<bool> LabutsShouldReturn;

};
