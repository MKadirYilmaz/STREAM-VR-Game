// Fill out your copyright notice in the Description page of Project Settings.


#include "MathMiniGame/MathGameSystem.h"
#include "MathMiniGame/MathBomb.h"
#include "MathMiniGame/MathScreenSystem.h"
#include "MathMiniGame/MathLaserShooter.h"
#include "BaseTimer.h"
#include "PhysicalButton.h"
#include "Kismet/KismetTextLibrary.h"


// Sets default values
AMathGameSystem::AMathGameSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomTimer = CreateDefaultSubobject<UBaseTimer>(TEXT("CustomTimer"));
}

// Called when the game starts or when spawned
void AMathGameSystem::BeginPlay()
{
	Super::BeginPlay();
	
	CustomTimer->StopCountdown.AddDynamic(this, &AMathGameSystem::TimeRunOut);
	for (APhysicalButton* _Button : TiedButtons)
	{
		_Button->ButtonPressed.AddDynamic(this, &AMathGameSystem::CheckAnswerIsCorrect);
	}
	for (APhysicalButton* _Button : TiedGameModeButtons)
	{
		_Button->ButtonPressed.AddDynamic(this, &AMathGameSystem::SetCurrentGameMode);
	}

	if (BombSpawnerActor)
	{
		for (UActorComponent* _ActorComp : BombSpawnerActor->GetComponents().Array())
		{
			USceneComponent* _SceneComp = Cast<USceneComponent>(_ActorComp);
			if (_SceneComp)
			{
				BombSpawnPoints.Add(_SceneComp);
			}
		}
	}
	if (StartButton)
	{
		StartButton->ButtonPressed.AddDynamic(this, &AMathGameSystem::StartMathGame);
	}
}

// Called every frame
void AMathGameSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GamePlaying)
	{
		if (TiedScreen)
		{
			TiedScreen->SetDuration(CustomTimer->GetRemainedTime());
		}
	}
}

void AMathGameSystem::CreateQuestionType1() // Find solution
{
	int32 _AnswerIndex;
	CreateExpression();
	if (FMath::RandBool()) // Dogru cevabin sag mi sol mu olacagini rastgele atar
	{
		SolChoice1 = Result;

		float _Abs = FMath::Abs(Result);
		SolChoice2 = FMath::RandRange(-_Abs, 2 * _Abs);
		while ((int32)SolChoice2 == (int32)SolChoice1) // If two options are the same, then repeat this process until they are different from each other
		{
			UE_LOG(LogTemp, Warning, TEXT("Two options become the same. Recalculating..."));
			SolChoice2 = FMath::RandRange(-_Abs - 2, 2 * _Abs + 2);
		}	
		_AnswerIndex = 0;
	}
	else 
	{
		SolChoice2 = Result;

		float _Abs = FMath::Abs(Result);
		SolChoice1 = FMath::RandRange(-_Abs, 2 * _Abs);
		while ((int32)SolChoice2 == (int32)SolChoice1) // If two options are the same, then repeat this process until they are different from each other
		{
			UE_LOG(LogTemp, Warning, TEXT("Two options become the same. Recalculating..."));
			SolChoice1 = FMath::RandRange(-_Abs - 2, 2 * _Abs + 2);
		}
		_AnswerIndex = 1;
	}
	FText _Ch1, _Ch2;
	TextConversion(SolChoice1, DifficultyLevel);
	_Ch1 = TextFromBlueprint;
	TextConversion(SolChoice2, DifficultyLevel);
	_Ch2 = TextFromBlueprint;
	SpawnMathBomb(Expression, _Ch1.ToString(), _Ch2.ToString(), GetDurationBasedOnDifficulty(), _AnswerIndex);
	
	//CustomTimer->StartCountdown(GetDurationBasedOnDifficulty(), this);
	ShowCountdown = true;
}

void AMathGameSystem::CreateQuestionType2() // Find biggest
{
	CreateExpression();
	float _Result1 = Result;
	ExpChoice1 = Expression;
	CreateExpression();
	float _Result2 = Result;
	ExpChoice2 = Expression;
	if (_Result1 == _Result2) // Bir sekilde iki secenekteki cevap ayni olursa soru olusturma isini en bastan tekrar yap
	{
		UE_LOG(LogTemp, Warning, TEXT("Two results are the same. Recalculating..."));
		CreateQuestionType2();
	}
	int32 _AnswerIndex = (_Result1 > _Result2) ? _AnswerIndex = 0 : _AnswerIndex = 1;
		
	SpawnMathBomb("Find the biggest solution", ExpChoice1, ExpChoice2, GetDurationBasedOnDifficulty(), _AnswerIndex);

	//CustomTimer->StartCountdown(GetDurationBasedOnDifficulty(), this);
	ShowCountdown = true;
}

void AMathGameSystem::CreateQuestionType3() // Find expression
{
	CreateExpression();
	float _Result1 = Result;
	ExpChoice1 = Expression;
	CreateExpression();
	float _Result2 = Result;
	ExpChoice2 = Expression;
	float _Answer;
	int32 _AnswerIndex;
	if (FMath::RandBool())
	{
		_AnswerIndex = 0;
		_Answer = _Result1;
	}
	else
	{
		_AnswerIndex = 1;
		_Answer = _Result2;
	}
	FText _Exp;
	TextConversion(_Answer, DifficultyLevel);
	_Exp = TextFromBlueprint;
	SpawnMathBomb(_Exp.ToString(), ExpChoice1, ExpChoice2, GetDurationBasedOnDifficulty(), _AnswerIndex);

	//CustomTimer->StartCountdown(GetDurationBasedOnDifficulty(), this);
	ShowCountdown = true;
}

void AMathGameSystem::NewQuestion()
{
	switch (MathGameMode)
	{
	case FindSolution:
		CreateQuestionType1();
		break;
	case FindBiggest:
		CreateQuestionType2();
		break;
	case FindExpression:
		CreateQuestionType3();
		break;
	default:
		break;
	}
	//SpawnMathBomb(Expression, ExpChoice1, ExpChoice2, GetDurationBasedOnDifficulty(), CurrentAnswerIndex);
}

void AMathGameSystem::SetTextFromBlueprint(FText BlueprintText)
{
	TextFromBlueprint = BlueprintText;
}

void AMathGameSystem::CreateExpression()
{
	Expression = "";
	Result = 0;
	TArray<float> Operands;
	TArray<int32> Operators;
	int32 _QuestionLength;
	switch (DifficultyLevel)
	{
	case 0:
		_QuestionLength = FMath::RandRange(2, 4); // Soruda max 2 ile 4 arasi sayi bulunabilir (2 sayi = 1 islem, 3 sayi = 2 islem, 4 sayi = 3 islem) 

		for (int i = 0; i < _QuestionLength; i++)
		{
			Operands.Add(FMath::RandRange(-25, 25)); // -25, 25 arasi tamsayilarla islem yapilir
		}
		for (int i = 0; i < _QuestionLength - 1; i++)
		{
			Operators.Add(FMath::RandRange(0, 3));
		}
		CombineOperandAndOperators(Operands, Operators);
		UE_LOG(LogTemp, Display, TEXT("New question had been made"));
		PrintQuestionToLog();
		break;
	case 1:
		_QuestionLength = FMath::RandRange(2, 4); // Soruda max 2 ile 4 arasi sayi bulunabilir (2 sayi = 1 islem, 3 sayi = 2 islem, 4 sayi = 3 islem) 

		for (int i = 0; i < _QuestionLength; i++)
		{
			Operands.Add((int)(FMath::RandRange(-25.f, 25.f) / 0.1f) * 0.1f); // -25.0, 25.0 arasi ondalikli sayilarla islem yapilir
		}
		for (int i = 0; i < _QuestionLength - 1; i++)
		{
			Operators.Add(FMath::RandRange(0, 3));
		}
		CombineOperandAndOperators(Operands, Operators);
		UE_LOG(LogTemp, Display, TEXT("New question had been made"));
		PrintQuestionToLog();
		break;
	case 2:
		_QuestionLength = FMath::RandRange(2, 4); // Soruda max 2 ile 4 arasi sayi bulunabilir (2 sayi = 1 islem, 3 sayi = 2 islem, 4 sayi = 3 islem) 

		for (int i = 0; i < _QuestionLength; i++)
		{
			Operands.Add((int)(FMath::RandRange(-30.f, 30.f) / 0.01f) * 0.01f); // -30.00, 30.00 arasi ondalikli sayilarla islem yapilir
		}
		for (int i = 0; i < _QuestionLength - 1; i++)
		{
			Operators.Add(FMath::RandRange(0, 3));
		}
		CombineOperandAndOperators(Operands, Operators);
		UE_LOG(LogTemp, Display, TEXT("New question had been made"));
		PrintQuestionToLog();
		break;
	default:
		break;
	}
}

float AMathGameSystem::CombineOperandAndOperators(const TArray<float>& Operands, const TArray<int32>& Operators)
{
	Result = 0;
	TArray<float> _Operands = Operands;
	TArray<int32> _Operators = Operators;
	UE_LOG(LogTemp, Warning, TEXT("Operand Start Length: %d, Operator Start Length: %d"), _Operands.Num(), _Operators.Num());
	PrintArray(_Operands);
	PrintArray(_Operators);
	for (int32 i = 0; i < _Operators.Num(); i++) // Islemler arasinda carpma veya bolme isareti varsa ilk onlari hallet
	{
		if (_Operators[i] == 2)	// Carpma
		{
			_Operators.RemoveAt(i);
			UE_LOG(LogTemp, Display, TEXT("Operators..."));
			PrintArray(_Operators);
			UE_LOG(LogTemp, Display, TEXT("Operands..."));
			PrintArray(_Operands);
			UE_LOG(LogTemp, Display, TEXT("Executing: %f * %f = %f"), _Operands[i], _Operands[i + 1], _Operands[i] * _Operands[i + 1]);
			_Operands[i] = _Operands[i] * _Operands[i + 1];
			UE_LOG(LogTemp, Display, TEXT("Operands..."));
			_Operands.RemoveAt(i + 1);
			PrintArray(_Operands);
			i--; // Operator ve operandlardan birer tane sildigimiz icin index numaramizi tekrar 1 azaltiyoruz.
		}
		else if (_Operators[i] == 3) // Bolme
		{
			_Operators.RemoveAt(i);
			UE_LOG(LogTemp, Display, TEXT("Operators..."));
			PrintArray(_Operators);
			UE_LOG(LogTemp, Display, TEXT("Operands..."));
			PrintArray(_Operands);
			UE_LOG(LogTemp, Display, TEXT("Executing: %f / %f = %f"), _Operands[i],  _Operands[i + 1], _Operands[i] / _Operands[i + 1]);
			_Operands[i] = _Operands[i] / _Operands[i + 1];
			UE_LOG(LogTemp, Display, TEXT("Operands..."));
			_Operands.RemoveAt(i + 1);
			PrintArray(_Operands);
			i--;

		}
	}
	for (int32 i = 0; i < _Operators.Num(); i++) // Ikici olarak toplama ve cikarma islemlerini yap
	{
		if (_Operators[i] == 0) // Toplama
		{
			_Operators.RemoveAt(i);
			UE_LOG(LogTemp, Display, TEXT("Operators..."));
			PrintArray(_Operators);
			UE_LOG(LogTemp, Display, TEXT("Operands..."));
			PrintArray(_Operands);
			UE_LOG(LogTemp, Display, TEXT("Executing: %f + %f = %f"), _Operands[i], _Operands[i + 1], _Operands[i] + _Operands[i + 1]);
			_Operands[i] = _Operands[i] + _Operands[i + 1];
			UE_LOG(LogTemp, Display, TEXT("Operands..."));
			PrintArray(_Operands);
			_Operands.RemoveAt(i + 1);
			i--;
		}
		else if (_Operators[i] == 1) // Cikarma
		{
			_Operators.RemoveAt(i);
			UE_LOG(LogTemp, Display, TEXT("Operators..."));
			PrintArray(_Operators);
			UE_LOG(LogTemp, Display, TEXT("Operands..."));
			PrintArray(_Operands);
			UE_LOG(LogTemp, Display, TEXT("Executing: %f - %f = %f"), _Operands[i], _Operands[i + 1], _Operands[i] - _Operands[i + 1]);
			_Operands[i] = _Operands[i] - _Operands[i + 1];
			UE_LOG(LogTemp, Display, TEXT("Operands..."));
			PrintArray(_Operands);
			_Operands.RemoveAt(i + 1);
			i--;
		}
	}
	
	if (_Operands.Num() != 0)
		Result = _Operands[0];
	else
		UE_LOG(LogTemp, Error, TEXT("Critical logical error. No operands remained for the result!"));
	
	bool _FirstPart = true;
	for (int32 i = 0; i < Operators.Num(); i++) // String ifadeyi olustur
	{
		FString _NewString;
		FText _Part1, _Part2;
		switch (Operators[i])
		{
		case 0:
			TextConversion(Operands[i], DifficultyLevel);
			_Part1 = TextFromBlueprint;
			TextConversion(Operands[i + 1], DifficultyLevel);
			_Part2 = TextFromBlueprint;
			_NewString = (_FirstPart) ? _Part1.ToString()
				+ " + " + _Part2.ToString() : " + " 
				+ _Part2.ToString();
			Expression += _NewString;
			break;
		case 1:
			TextConversion(Operands[i], DifficultyLevel);
			_Part1 = TextFromBlueprint;
			TextConversion(Operands[i + 1], DifficultyLevel);
			_Part2 = TextFromBlueprint;
			_NewString = (_FirstPart) ? _Part1.ToString()
				+ " - " + _Part2.ToString() : " - " 
				+ _Part2.ToString();
			Expression += _NewString;
			break;
		case 2:
			TextConversion(Operands[i], DifficultyLevel);
			_Part1 = TextFromBlueprint;
			TextConversion(Operands[i + 1], DifficultyLevel);
			_Part2 = TextFromBlueprint;
			_NewString = (_FirstPart) ? _Part1.ToString()
				+ " * " + _Part2.ToString() : " * " 
				+ _Part2.ToString();
			Expression += _NewString;
			break;
		case 3:
			TextConversion(Operands[i], DifficultyLevel);
			_Part1 = TextFromBlueprint;
			TextConversion(Operands[i + 1], DifficultyLevel);
			_Part2 = TextFromBlueprint;
			_NewString = (_FirstPart) ? _Part1.ToString()
				+ " / " + _Part2.ToString() : " / "
				+ _Part2.ToString();
			Expression += _NewString;
			break;
		default:
			break;
		}
		_FirstPart = false;
	}
	return Result;
}

void AMathGameSystem::PrintQuestionToLog()
{
	UE_LOG(LogTemp, Display, TEXT("Question: %s, Result: %f"), *Expression, Result);
}

void AMathGameSystem::TimeRunOut(AActor* Self)
{
	if (Self == this)
	{
		EndMathGame();
	}
}

void AMathGameSystem::SetupScreenAndButtons(FString Screen, FString C1, FString C2, int32 AnsIndex, AMathBomb* SelfBomb)
{
	LockedBomb = SelfBomb;
	CurrentAnswerIndex = AnsIndex;
	UE_LOG(LogTemp, Display, TEXT("Button1 Text: %s, Button2 Text: %s, AnswerIndex: %d"), *C1, *C2, AnsIndex);
	if (TiedButtons.Num() >= 2)
	{
		TiedButtons[0]->SetButtonText(C1);
		TiedButtons[1]->SetButtonText(C2);
	}
	if (TiedScreen)
	{
		TiedScreen->SetupTexts(Screen, C1, C2);
	}
}

void AMathGameSystem::StartMathGame(int32 ButtonIndex)
{
	if (GamePlaying)
		return;
	MusicSystem(true);
	MaxHealth = 100;
	CurrentTotalScore = 0;
	GamePlaying = true;
	if (TiedScreen)
	{
		TiedScreen->SetScore(CurrentTotalScore);
		TiedScreen->SetHealth(MaxHealth);
	}
	switch (DifficultyLevel)
	{
	case 0: // Easy
		CurrentScoreMultiplier = EasyScoreMultiplier;
		break;
	case 1:
		CurrentScoreMultiplier = NormalScoreMultiplier;
		break;
	case 2:
		CurrentScoreMultiplier = HardScoreMultiplier;
		break;
	}
	CustomTimer->StartCountdown(60.f, this);
	GetWorldTimerManager().SetTimer(QuestionCreator, this, &AMathGameSystem::NewQuestion, GetSpawnRateBasedOnDifficulty(), true);
}

void AMathGameSystem::EndMathGame()
{
	MusicSystem(false);
	GamePlaying = false;
	if (TiedScreen)
	{
		if (MaxHealth < 1.f) // Oyun, karakterin cani bittigi icin bitmisse
		{
			TiedScreen->LooseScreen(CurrentTotalScore);
		}
		else
		{
			CurrentTotalScore = CurrentTotalScore * ((MaxHealth + 10) / 10);
			TiedScreen->FinishedGame(CurrentTotalScore);
		}
	}
	GetWorldTimerManager().PauseTimer(QuestionCreator); // Yeni soru olusturmayi birak
	AutoSaveSystem(CurrentTotalScore);
	GameFinished.Broadcast();
	/*
	for (AMathBomb* _Bomb : FlyingBombs)
	{
		if(_Bomb) // Double Check
		_Bomb->Destroy();
	}
	*/
}

void AMathGameSystem::BombDurationRunOut(AMathBomb* SelfBomb)
{
	if (GamePlaying && TiedScreen)
	{
		MaxHealth -= 15;
		TiedScreen->SetHealth(MaxHealth);
		if (MaxHealth <= 0)
		{
			EndMathGame();
		}
		else if (TiedLaserShooter)
		{
			if(TiedLaserShooter->GetLockedBomb() == SelfBomb) // Sadece kilitliyse suresi biten bombanin mesajini goster
				TiedScreen->BombDurationRunOutScreen();
		}
	}
}

void AMathGameSystem::BombFalseAnswer()
{
	if (GamePlaying && TiedScreen)
	{
		MaxHealth -= 25;
		TiedScreen->SetHealth(MaxHealth);
		if (MaxHealth <= 0)
		{
			EndMathGame();
		}
		else
		{
			TiedScreen->WrongAnswerScreen();
		}
	}
}

void AMathGameSystem::DeleteBombFromArray(AMathBomb* SelfBomb)
{
	/*
	for (int32 i = 0; i < FlyingBombs.Num(); i++)
	{
		if (FlyingBombs[i] == SelfBomb)
		{
			FlyingBombs.RemoveAt(i);
		}
	}
	*/
}

void AMathGameSystem::ReleaseLaserShooter()
{
	if (TiedLaserShooter)
	{
		TiedLaserShooter->SetFreeLaser();
	}
}

void AMathGameSystem::CheckAnswerIsCorrect(int32 ButtonIndex)
{
	if (ButtonIndex == CurrentAnswerIndex) // Dogruysa
	{
		if (LockedBomb && TiedScreen && TiedLaserShooter)
		{
			TiedLaserShooter->FireLaser(ButtonIndex);
			LockedBomb->EliminateBomb();
			TiedScreen->ResetScreen();
			CurrentTotalScore += 10.f * CurrentScoreMultiplier;
			UE_LOG(LogTemp, Display, TEXT("Soruyu dogru bildiniz. Toplam Score: %f"), CurrentTotalScore);
			TiedScreen->SetScore(CurrentTotalScore);
		}
	}
	else if(TiedLaserShooter && TiedScreen) // Yanlissa
	{
		TiedLaserShooter->FireLaser(ButtonIndex);
		UE_LOG(LogTemp, Warning, TEXT("Yanlis cevap!!"));
		LockedBomb->FastDropBomb();
		BombFalseAnswer();
	}
}

float AMathGameSystem::GetDurationBasedOnDifficulty()
{
	switch (DifficultyLevel)
	{
	case 0:
		return EasyModeDuration;
		break;
	case 1:
		return NormalModeDuration;
		break;
	case 2:
		return HardModeDuration;
		break;
	default:
		return 0.f;
		break;
	}
}

float AMathGameSystem::GetSpawnRateBasedOnDifficulty()
{
	switch (DifficultyLevel)
	{
	case 0:
		return EasyModeSpawnRate;
	case 1:
		return NormalModeSpawnRate;
	case 2:
		return HardModeSpawnRate;
	default:
		return EasyModeSpawnRate;
	}
}

int32 AMathGameSystem::GetCurrentGameMode() const
{
	switch (MathGameMode)
	{
	case FindSolution:
		return 0;
	case FindBiggest:
		return 1;
	case FindExpression:
		return 2;
	default:
		return 0;
	}
}

void AMathGameSystem::SetCurrentGameMode(int32 GameModeIndex)
{
	if (!GamePlaying) // Game mode cannot change while playing
	{
		switch (GameModeIndex)
		{
		case 0:
			MathGameMode = EMathGameTypes::FindSolution;
			break;
		case 1:
			MathGameMode = EMathGameTypes::FindBiggest;
			break;
		case 2:
			MathGameMode = EMathGameTypes::FindExpression;
		}
	}
}

void AMathGameSystem::SpawnMathBomb(FString MainExpression, FString Choice1, FString Choice2, float Duration, int32 AnswerIndex)
{
	int32 _RandIndex = FMath::RandRange(0, BombSpawnPoints.Num() - 1);
	if (!BombSpawnPoints.IsEmpty() && MathBombClass)
	{
		AMathBomb* _Bomb = GetWorld()->SpawnActor<AMathBomb>(MathBombClass, BombSpawnPoints[_RandIndex]->GetComponentLocation(), FRotator(0, 0, 0));
		_Bomb->SetActorScale3D(FVector(0.75f, 0.75f, 0.75f));
		_Bomb->SetMainExpression(MainExpression);
		_Bomb->SetChoice1(Choice1);
		_Bomb->SetChoice2(Choice2);
		_Bomb->SetDuration(Duration);
		_Bomb->SetAnswerIndex(AnswerIndex);
		_Bomb->SetMathGameManager(this);
		_Bomb->SetAddDynamic();
		//FlyingBombs.Add(_Bomb);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There is no bomb spawn point! MathGameSystem/SpawnMathBomb"));
		return;
	}
}

template <typename T>
void AMathGameSystem::PrintArray(TArray<T> Array)
{
	int32 i = 0;
	for (T Value : Array)
	{
		UE_LOG(LogTemp, Display, TEXT("%d: %f"), i, (float) Value);
		i++;
	}
}
