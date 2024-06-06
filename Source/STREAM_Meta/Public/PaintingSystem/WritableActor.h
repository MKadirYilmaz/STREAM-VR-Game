// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WritableActor.generated.h"

class UTextureRenderTarget2D;
class UMaterialInstanceDynamic;
UCLASS()
class STREAM_META_API AWritableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWritableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
		void SetupMaterialsInBlueprint();
	UFUNCTION(BlueprintImplementableEvent)
		void CreateCustomRenderTargets2D();

	//void AssignRenderTargets();
	//UFUNCTION(BlueprintCallable)
	//	UTextureRenderTarget2D* GetCorrectRenderTarget2D(int32 Index);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* WritableMesh;

	UMaterialInstanceDynamic* BrushMaterial;

	UTextureRenderTarget2D* RenderTarget;

	UPROPERTY(EditAnywhere)
		UMaterialInstance* PenMaterialBase;
	UPROPERTY(BlueprintReadWrite)
		UMaterialInstanceDynamic* PenMaterial;
	UPROPERTY(EditAnywhere)
		UMaterialInstance* BoardMaterialBase;
	UPROPERTY(BlueprintReadWrite)
		UMaterialInstanceDynamic* BoardMaterial;
	UPROPERTY(BlueprintReadWrite)
		UTextureRenderTarget2D* RenderTarget2D;


		// New Painting System
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UTextureRenderTarget2D*> TextureRenderTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FName> RenderTargetParamNames;

public:

	UFUNCTION(BlueprintCallable, Category = "Setters")
		void SetBrushMaterial(UMaterialInstanceDynamic* NewBrushMaterial) { BrushMaterial = NewBrushMaterial; }
	UFUNCTION(BlueprintCallable, Category = "Getters")
		UMaterialInstanceDynamic* GetBrushMaterial() const { return BrushMaterial; }

	UFUNCTION(BlueprintCallable, Category = "Setters")
	void SetRenderTarget2D(UTextureRenderTarget2D* NewRenderTarget) { RenderTarget = NewRenderTarget; }
	UFUNCTION(BlueprintCallable, Category = "Getters")
	UTextureRenderTarget2D* GetTextureRenderTarget2D() const { return RenderTarget; }
};
