// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagers/SoundManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASoundManager::ASoundManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASoundManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USoundBase* ASoundManager::GetSoundFromTag(FName SurfaceTag1, FName SurfaceTag2)
{
	FName _Combined = FName(SurfaceTag1.ToString() +"&" + SurfaceTag2.ToString());
	FName _AltCombined = FName(SurfaceTag2.ToString() + "&" + SurfaceTag1.ToString());
	for (FTagSounds _TagSound : AllSoundFX)
	{
		if (_TagSound.CombinedTagNames.IsEqual(_Combined) || _TagSound.CombinedTagNames.IsEqual(_AltCombined)) // T1&T2 == T2&T1
		{
			return _TagSound.TagSound;
		}
	}
	return DefaultCollisionSound;
}

void ASoundManager::CalculateAndPlaySound(FName Obj1SurfaceTag, FName Obj2SurfaceTag, FVector HitImpulse, FVector HitLocation)
{
	float _Volume = VolumeMultiplier * (HitImpulse.Length() / 5000.f);
	_Volume = FMath::Clamp(_Volume, 0.01f, 0.75f); // Sesi cok yuksek veya cok dusuk olmaktan koru
	UGameplayStatics::PlaySoundAtLocation(this, GetSoundFromTag(Obj1SurfaceTag, Obj2SurfaceTag), HitLocation, _Volume);
	UE_LOG(LogTemp, Display, TEXT("Sound played: Volume: %f"), _Volume);
}

