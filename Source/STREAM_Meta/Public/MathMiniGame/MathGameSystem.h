// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MathGameSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameFinished);

class APhysicalButton;
UENUM(BlueprintType)
enum EMathGameTypes
{
	//UPROPERTY(EditAnywhere)
	FindSolution,
	//UPROPERTY(EditAnywhere)
	FindBiggest,
	//UPROPERTY(EditAnywhere)
	FindExpression
};

UCLASS()
class STREAM_META_API AMathGameSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMathGameSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FOnGameFinished GameFinished;

	UFUNCTION(BlueprintCallable)
	// Find solution type of questions
	void CreateQuestionType1();
	UFUNCTION(BlueprintCallable)
	// Find biggest choice type of questions
	void CreateQuestionType2();
	UFUNCTION(BlueprintCallable)
	// Find the expression type of questions
	void CreateQuestionType3();

	UFUNCTION(BlueprintCallable)
	void NewQuestion();
	UFUNCTION(BlueprintImplementableEvent)
	void AutoSaveSystem(float Score);

	UFUNCTION(BlueprintImplementableEvent)
	void TextConversion(float Value, int32 MaxFractionalDigits);
	UFUNCTION(BlueprintImplementableEvent)
	void MusicSystem(bool bStart);
	UFUNCTION(BlueprintCallable)
	void SetTextFromBlueprint(FText BlueprintText);


	UFUNCTION(BlueprintCallable)
	void CreateExpression();

	float CombineOperandAndOperators(const TArray<float>& Operands, const TArray<int32>& Operators);
	
	template <typename T>
	void PrintArray(TArray<T> Array);
	
	UFUNCTION(BlueprintCallable)
	void PrintQuestionToLog();

	void SpawnMathBomb(FString MainExpression, FString Choice1, FString Choice2, float Duration, int32 AnswerIndex);
	void SetupScreenAndButtons(FString Screen, FString C1, FString C2, int32 AnsIndex, class AMathBomb* SelfBomb);

	UFUNCTION(BlueprintCallable)
	void StartMathGame(int32 ButtonIndex);
	UFUNCTION(BlueprintCallable)
	void EndMathGame();

	void BombDurationRunOut(AMathBomb* SelfBomb);
	void DeleteBombFromArray(AMathBomb* SelfBomb);
	void BombFalseAnswer();
	void ReleaseLaserShooter();
private:
	UFUNCTION()
	void TimeRunOut(AActor* Self);
	UFUNCTION()
	void CheckAnswerIsCorrect(int32 ButtonIndex);
	float GetDurationBasedOnDifficulty();
	float GetSpawnRateBasedOnDifficulty();
	class UBaseTimer* CustomTimer;

	UPROPERTY(EditAnywhere, Category = "Gameplay Modes")
	// 0 Kolay, 1 Normal, 2 Zor
	int32 DifficultyLevel = 0;
	UPROPERTY(EditAnywhere, Category = "Gameplay Modes")
	TEnumAsByte<EMathGameTypes> MathGameMode = EMathGameTypes::FindSolution;


	UPROPERTY(EditAnywhere)
	// Icinde birkac scene component olan ve bu component pozisyonlarini bombalari spawn etmek icin kullanacagimiz actor.
	AActor* BombSpawnerActor;
	TArray<USceneComponent*> BombSpawnPoints;
	TArray<AMathBomb*> FlyingBombs;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMathBomb> MathBombClass;

	UPROPERTY(EditAnywhere)
	TArray<APhysicalButton*> TiedButtons;
	UPROPERTY(EditAnywhere)
	TArray<APhysicalButton*> TiedGameModeButtons;

	UPROPERTY(EditAnywhere)
	APhysicalButton* StartButton;

	UPROPERTY(EditAnywhere)
	class AMathScreenSystem* TiedScreen;
	UPROPERTY(EditAnywhere)
	class AMathLaserShooter* TiedLaserShooter;

	class AMathBomb* LockedBomb;

	UPROPERTY(EditAnywhere, Category = "Gameplay Adjustments")
	float EasyModeDuration = 25.f;
	UPROPERTY(EditAnywhere, Category = "Gameplay Adjustments")
	float NormalModeDuration = 17.5f;
	UPROPERTY(EditAnywhere, Category = "Gameplay Adjustments")
	float HardModeDuration = 12.5f;

	UPROPERTY(EditAnywhere, Category = "Gameplay Adjustments")
	float EasyModeSpawnRate = 8.5f;
	UPROPERTY(EditAnywhere, Category = "Gameplay Adjustments")
	float NormalModeSpawnRate = 8.f;
	UPROPERTY(EditAnywhere, Category = "Gameplay Adjustments")
	float HardModeSpawnRate = 7.5f;

	FText TextFromBlueprint;
	FString Expression = "";
	float Result = 0;

	int32 ScoreOfQuestion = 0;
	float CurrentTotalScore = 0.f;
	int32 MaxHealth = 100;
	int32 CurrentAnswerIndex = 0;
	bool ShowCountdown = false;

	float SolChoice1 = 0;
	float SolChoice2 = 0;

	FString ExpChoice1 = "";
	FString ExpChoice2 = "";

	float CurrentScoreMultiplier;
	float EasyScoreMultiplier = 1.f;
	float NormalScoreMultiplier = 1.35f;
	float HardScoreMultiplier = 1.6f;

	FTimerHandle QuestionCreator;

	bool GamePlaying = false;
public:
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentGameMode() const;

	FString GetExpression() { return Expression; }
	class AMathLaserShooter* GetTiedLaserShooter() { return TiedLaserShooter; }

	void SetDifficulty(int32 NewDifficulty) { DifficultyLevel = NewDifficulty; }
	UFUNCTION(BlueprintCallable)
	// Find Solution = 0, Find Biggest = 1, Find Expression = 2 (Runtime selection)
	void SetCurrentGameMode(int32 GameModeIndex);

	

};
