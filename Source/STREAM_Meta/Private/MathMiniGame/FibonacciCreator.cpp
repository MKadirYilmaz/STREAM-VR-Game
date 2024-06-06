// All rights belong to Kadir Yilmaz


#include "MathMiniGame/FibonacciCreator.h"
#include "PhysicalButton.h"

// Sets default values for this component's properties
UFibonacciCreator::UFibonacciCreator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFibonacciCreator::BeginPlay()
{
	Super::BeginPlay();

	if (TiedButtonNext)
	{
		TiedButtonNext->ButtonPressed.AddDynamic(this, &UFibonacciCreator::GetNextFibonacciNumber);
	}
	if (TiedButtonPrev)
	{
		TiedButtonPrev->ButtonPressed.AddDynamic(this, &UFibonacciCreator::GetPrevFibonacciNumber);
	}
	
}


// Called every frame
void UFibonacciCreator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void UFibonacciCreator::GetNextFibonacciNumber(int32 ButtonIndex)
{
	if (CurrentFibonacciIndex > 90)
		return;
	PrevFibonacci = Number1; // 0 = 1 1 -> 1 = 1 2 -> 1 = 2 3 -> 2 = 3 5 -> 3 = 5 8
	Number1 = Number1 + Number2;

	int64 _Temp = Number1;
	Number1 = Number2;
	Number2 = _Temp;

	CurrentFibonacciNumber = Number2;
	CurrentFibonacciIndex++;
	RefreshText();
}

void UFibonacciCreator::GetPrevFibonacciNumber(int32 ButtonIndex)
{
	UE_LOG(LogTemp, Display, TEXT("Prev: %d, Num1: %d, Num2: %d"), PrevFibonacci, Number1, Number2);
	if (PrevFibonacci <= 0)
		return;
	
	int64 _Temp = PrevFibonacci;
	
	Number2 = Number1;
	Number1 = _Temp;
	
	PrevFibonacci = Number2 - Number1;


	CurrentFibonacciNumber = Number2;
	if(CurrentFibonacciIndex > 1)
		CurrentFibonacciIndex--;
	RefreshText();
	UE_LOG(LogTemp, Display, TEXT("Second Part: Prev: %d, Num1: %d, Num2: %d"), PrevFibonacci, Number1, Number2);
}
