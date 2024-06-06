// All rights belong to Muhammed Kadir Yilmaz

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TriangleCreator.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STREAM_META_API UTriangleCreator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTriangleCreator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetInitialScales(USceneComponent* Start, USceneComponent* End);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ResetComponent();
	UFUNCTION(BlueprintCallable)
	void SetupComponent(USceneComponent* Start, UStaticMeshComponent* Laser1, UStaticMeshComponent* Laser2, UStaticMeshComponent* Laser3);
	void DeformEdges(float Edge1Value, float Edge2Value);

private:
	UPROPERTY(EditAnywhere)
	class ALeverSystem* TiedEdge1Lever;

	UPROPERTY(EditAnywhere)
	class ALeverSystem* TiedEdge2Lever;

	USceneComponent* StartingComp;
	USceneComponent* PointComp1;

	UPROPERTY(EditAnywhere)
	float Line1Length;
	UPROPERTY(EditAnywhere)
	float Line2Length;
	UPROPERTY(EditAnywhere)
	float MaxLine1Length = 1000.f;
	UPROPERTY(EditAnywhere)
	float MaxLine2Length = 1000.f;

	FVector EndingPoint1;
	FVector EndingPoint2;

	UPROPERTY(VisibleAnywhere)
	float InitialLaserScale;

	UStaticMeshComponent* Laser1Mesh;
	UStaticMeshComponent* Laser2Mesh;
	UStaticMeshComponent* Laser3Mesh;

	FVector Laser1Scale;
	FVector Laser2Scale;
	FVector Laser3Scale;

public:

	UFUNCTION(BlueprintCallable)
	void SetStartPoint(USceneComponent* Point1) { StartingComp = Point1; }
	UFUNCTION(BlueprintCallable)
	void SetPoint2(USceneComponent* Point2) { PointComp1 = Point2; }


	UFUNCTION(BlueprintCallable)
	void SetLaser1Mesh(UStaticMeshComponent* LaserMesh) { Laser1Mesh = LaserMesh; }
	UFUNCTION(BlueprintCallable)
	void SetLaser2Mesh(UStaticMeshComponent* LaserMesh) { Laser2Mesh = LaserMesh; }
	UFUNCTION(BlueprintCallable)
	void SetLaser3Mesh(UStaticMeshComponent* LaserMesh) { Laser3Mesh = LaserMesh; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetLaser1Scale() const { return Laser1Scale; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetLaser2Scale() const { return Laser2Scale; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetLaser3Scale() const { return Laser3Scale; }


};

