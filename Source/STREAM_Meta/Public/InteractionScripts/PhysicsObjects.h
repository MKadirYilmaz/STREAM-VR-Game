// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsObjects.generated.h"


class AMetaMode;
UCLASS()
class STREAM_META_API APhysicsObjects : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhysicsObjects();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void RepositionGrabbingPoint(const FVector& NewImpactPoint);
	// PhysicsObjects'in asil meshini ParentComp'a baglar. KeepWorld kurali uygulanir
	void AttachTheObjectToComponent(USceneComponent* ParentComp); 
	// Ana fonksiyonun asiri yuklenmis versiyonu. ParentComp icindeki sockete baglanabilmeyi saglar
	void AttachTheObjectToComponent(USceneComponent* ParentComp, FName SocketName); 

	void BreakAttachmentWithComponent();

	void SetupGrabPointRotation(const FVector& ImpactNormal);
protected:
	UFUNCTION()
	void OnMeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ProtectSound();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Mesh;

	AMetaMode* CurrentGameMode;

	FTransform SocketTransform;
	FVector HitPoint;

	bool SoundPlayed = false;
	FTimerHandle SoundProtector;

public: // Getter ve setter fonkisyonlar
	UStaticMeshComponent* GetBodyMesh() { return Mesh; }

	FTransform GetGrabPointSocketTransform();
	void SetImpactPoint(const FVector& ImpactPoint) { HitPoint = ImpactPoint; RepositionGrabbingPoint(ImpactPoint); }

	

};
