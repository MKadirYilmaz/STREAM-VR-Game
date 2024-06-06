// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MathBomb.generated.h"

UCLASS()
class STREAM_META_API AMathBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMathBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SetDuration(float MaxDuration);

	UFUNCTION(BlueprintImplementableEvent)
	void ExplosionSystem();

	void EliminateBomb();

	void DestroyTheBomb();
	void DestroyTheBombPunishment();
	void FastDropBomb();

private:
	UFUNCTION()
	void GameFinishedDestroy();
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BombMesh;

	class AMathGameSystem* MathGameManager;
	FTimerHandle ExplosionHandler;
	
	UPROPERTY(EditAnywhere)
	float SurfaceZ = 600.f;

	float FallingSpeed;
	float Duration = 10.f;
	int32 AnswerIndex;

	FString MainExpression, Choice1, Choice2;

	FHitResult SurfaceDetector;

public:
	void SetAddDynamic();

	void SetChoice1(FString Expression) { Choice1 = Expression; }
	void SetChoice2(FString Expression) { Choice2 = Expression; }
	void SetMainExpression(FString Expression) { MainExpression = Expression; }
	void SetAnswerIndex(int32 Index) { AnswerIndex = Index; }

	void SetMathGameManager(class AMathGameSystem* GameManager) { MathGameManager = GameManager; }

	FString GetChoice1() const { return Choice1; }
	FString GetChoice2() const { return Choice2; }
	FString GetMainExpression() const { return MainExpression; }
	int32 GetAnswerIndex() const { return AnswerIndex; }
};
