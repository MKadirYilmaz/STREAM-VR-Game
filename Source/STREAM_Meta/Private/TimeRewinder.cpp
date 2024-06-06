// All rights belong to Muhammed Kadir Yilmaz


#include "TimeRewinder.h"

// Sets default values for this component's properties
UTimeRewinder::UTimeRewinder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTimeRewinder::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTimeRewinder::PrepareRewind()
{
	UStaticMeshComponent* _PhysicsMesh = GetOwner()->GetComponentByClass<UStaticMeshComponent>();
	if (_PhysicsMesh)
	{
		_PhysicsMesh->SetSimulatePhysics(false);
		IsRewinding = true;
	}
}

void UTimeRewinder::StartRewind(float IndexValue, float DeltaTime)
{
	float _Index = FMath::Clamp(IndexValue, 0.f, 1.f);
	int32 _ArrayIndex = (int32)((float)Snapshots.Num() * _Index);
	if (Snapshots.Num() > _ArrayIndex)
	{
		GetOwner()->SetActorLocation(FMath::VInterpTo(GetOwner()->GetActorLocation(), Snapshots[_ArrayIndex].GetLocation(), DeltaTime, 2.f));
		GetOwner()->SetActorRotation(FMath::RInterpTo(GetOwner()->GetActorRotation(), Snapshots[_ArrayIndex].GetRotation().Rotator(), DeltaTime, 2.f));
		ExecutedArrayIndex = _ArrayIndex;
	}
}

void UTimeRewinder::ExecuteRewind()
{
	if (LinearVelocities.Num() > ExecutedArrayIndex && RadialVelocities.Num() > ExecutedArrayIndex)
	{
		UStaticMeshComponent* _PhysicsMesh = GetOwner()->GetComponentByClass<UStaticMeshComponent>();
		if(_PhysicsMesh)
		{
			_PhysicsMesh->SetSimulatePhysics(true);
			_PhysicsMesh->SetPhysicsLinearVelocity(LinearVelocities[ExecutedArrayIndex]);
			_PhysicsMesh->SetPhysicsAngularVelocityInRadians(RadialVelocities[ExecutedArrayIndex]);
			IsRewinding = false;
			int32 RemoveAmount = Snapshots.Num() - ExecutedArrayIndex;
			for (int32 i = 0; i < RemoveAmount; i++) // Delete all datas after executed time
			{
				if (ExecutedArrayIndex < Snapshots.Num() && ExecutedArrayIndex < LinearVelocities.Num() && ExecutedArrayIndex < RadialVelocities.Num())
				{
					Snapshots.RemoveAt(ExecutedArrayIndex);
					LinearVelocities.RemoveAt(ExecutedArrayIndex);
					RadialVelocities.RemoveAt(ExecutedArrayIndex);
				}
			}
		}
	}
}

// Called every frame
void UTimeRewinder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (!SavedThisTick && ShouldTakeSnaps && !IsRewinding) // Her iki tickte bir kayit alacak
	{
		UStaticMeshComponent* _PhysicsMesh = GetOwner()->GetComponentByClass<UStaticMeshComponent>();
		if (Snapshots.Num() < 5000)
		{
			Snapshots.Push(GetOwner()->GetActorTransform());
			if (_PhysicsMesh)
			{
				LinearVelocities.Push(_PhysicsMesh->GetPhysicsLinearVelocity());
				RadialVelocities.Push(_PhysicsMesh->GetPhysicsAngularVelocityInRadians());
			}
		}
		else // Yeterince veri toplandiysa ilk verileri silip yenileri yazmaya devam et
		{
			Snapshots.RemoveAt(0);
			Snapshots.Push(GetOwner()->GetActorTransform());
			if (_PhysicsMesh)
			{
				LinearVelocities.RemoveAt(0);
				LinearVelocities.Push(_PhysicsMesh->GetPhysicsLinearVelocity());
				RadialVelocities.RemoveAt(0);
				RadialVelocities.Push(_PhysicsMesh->GetPhysicsAngularVelocityInRadians());
			}
		}
		SavedThisTick = true;
		return;
	}
	SavedThisTick = false;
	// ...
}

