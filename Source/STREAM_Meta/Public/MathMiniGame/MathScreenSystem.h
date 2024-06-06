// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MathScreenSystem.generated.h"


class UTextRenderComponent;
UCLASS()
class STREAM_META_API AMathScreenSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMathScreenSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetupTexts(const FString& Expression, const FString& Button1, const FString& Button2);

	UFUNCTION(BlueprintImplementableEvent)
	void SetScore(float Score);

	UFUNCTION(BlueprintImplementableEvent)
	void SetHealth(int32 Health);

	UFUNCTION(BlueprintImplementableEvent)
	void SetDuration(float Duration);

	UFUNCTION(BlueprintCallable)
	void RecalculateTextSizes();

	void ResetScreen();
	void WrongAnswerScreen();
	void BombDurationRunOutScreen();
	void LooseScreen(float Score);
	void FinishedGame(float Score);
private:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ScreenMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* ExpressionRenderer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* Button1Renderer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* Button2Renderer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* ScoreRenderer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* HealthRenderer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* RemainedDuration;


	float DefaultTextWordSize = 26.f;
};
