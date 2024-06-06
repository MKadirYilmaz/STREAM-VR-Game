// Fill out your copyright notice in the Description page of Project Settings.


#include "MathMiniGame/MathScreenSystem.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AMathScreenSystem::AMathScreenSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ScreenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScreenMesh"));
	ExpressionRenderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ExpressionRenderer"));
	Button1Renderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Button1Renderer"));
	Button2Renderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Button2Renderer"));

	ScoreRenderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreRenderer"));
	HealthRenderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthRenderer"));

	RemainedDuration = CreateDefaultSubobject<UTextRenderComponent>(TEXT("RemainedDuration"));

	ScreenMesh->SetupAttachment(RootComponent);
	ExpressionRenderer->SetupAttachment(ScreenMesh);
	Button1Renderer->SetupAttachment(ScreenMesh);
	Button2Renderer->SetupAttachment(ScreenMesh);

	ScoreRenderer->SetupAttachment(ScreenMesh);
	HealthRenderer->SetupAttachment(ScreenMesh);

	RemainedDuration->SetupAttachment(ScreenMesh);

}

// Called when the game starts or when spawned
void AMathScreenSystem::BeginPlay()
{
	Super::BeginPlay();
	
	SetDuration(60.f);
	SetScore(0.f);
	SetHealth(100);
}

// Called every frame
void AMathScreenSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMathScreenSystem::ResetScreen()
{
	ExpressionRenderer->SetText(FText::FromString("Waiting..."));
	Button1Renderer->SetText(FText::FromString(""));
	Button2Renderer->SetText(FText::FromString(""));
	RecalculateTextSizes();
}

void AMathScreenSystem::WrongAnswerScreen()
{
	ExpressionRenderer->SetText(FText::FromString("Your Answer Is Wrong!.."));
	Button1Renderer->SetText(FText::FromString(""));
	Button2Renderer->SetText(FText::FromString(""));
	RecalculateTextSizes();
}

void AMathScreenSystem::BombDurationRunOutScreen()
{
	ExpressionRenderer->SetText(FText::FromString("Bomb touched the surface!"));
	Button1Renderer->SetText(FText::FromString(""));
	Button2Renderer->SetText(FText::FromString(""));
	RecalculateTextSizes();
}

void AMathScreenSystem::LooseScreen(float Score)
{
	FString _Expression = "Game over! Your score: " + FString::SanitizeFloat(Score);
	ExpressionRenderer->SetText(FText::FromString(_Expression));
	Button1Renderer->SetText(FText::FromString(""));
	Button2Renderer->SetText(FText::FromString(""));
	RecalculateTextSizes();
}

void AMathScreenSystem::FinishedGame(float Score)
{
	FString _Expression = "Game Finished! Your score: " + FString::SanitizeFloat(Score);
	ExpressionRenderer->SetText(FText::FromString(_Expression));
	Button1Renderer->SetText(FText::FromString(""));
	Button2Renderer->SetText(FText::FromString(""));
	RecalculateTextSizes();
}

void AMathScreenSystem::RecalculateTextSizes()
{
	float _Multiplier = 0.f;
	_Multiplier = 100.f / ExpressionRenderer->GetTextLocalSize().Y;
	float _NewSizeExpression = ExpressionRenderer->WorldSize * _Multiplier;
	ExpressionRenderer->SetWorldSize(_NewSizeExpression);

	/*
	_Multiplier = 70.f / (Button1Renderer->GetTextLocalSize().Y + Button2Renderer->GetTextLocalSize().Y);
	float _NewSize1 = Button1Renderer->WorldSize * _Multiplier;
	float _NewSize2 = Button2Renderer->WorldSize * _Multiplier;

	_NewSize1 = (_NewSize1 <= 0) ? DefaultTextWordSize : _NewSize1;
	_NewSize2 = (_NewSize2 <= 0) ? DefaultTextWordSize : _NewSize2;

	Button1Renderer->SetWorldSize(_NewSize1);
	Button2Renderer->SetWorldSize(_NewSize2);
	
	*/
}



