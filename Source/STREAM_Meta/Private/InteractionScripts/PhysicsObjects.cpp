// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionScripts/PhysicsObjects.h"
#include "Kismet/GameplayStatics.h"
#include "GameManagers/MetaMode.h"
#include "GameManagers/SoundManager.h"


// Sets default values
APhysicsObjects::APhysicsObjects()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(true);

}

// Called when the game starts or when spawned
void APhysicsObjects::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentHit.AddDynamic(this, &APhysicsObjects::OnMeshComponentHit);
	CurrentGameMode = Cast<AMetaMode>(UGameplayStatics::GetGameMode(this));
}

// Called every frame
void APhysicsObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APhysicsObjects::RepositionGrabbingPoint(const FVector& NewImpactPoint)
{
	
}

void APhysicsObjects::AttachTheObjectToComponent(USceneComponent* ParentComp)
{
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
	Mesh->AttachToComponent(ParentComp, 
		FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true));
}

void APhysicsObjects::AttachTheObjectToComponent(USceneComponent* ParentComp, FName SocketName)
{
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
	Mesh->AttachToComponent(ParentComp,
		FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true), SocketName);
}

void APhysicsObjects::BreakAttachmentWithComponent()
{
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
}

void APhysicsObjects::SetupGrabPointRotation(const FVector& ImpactNormal)
{
	FRotator _TargetRotation = ImpactNormal.Rotation();
}

void APhysicsObjects::OnMeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (NormalImpulse.Length() / Mesh->GetMass() > 150.f)
	{
		UE_LOG(LogTemp, Display, TEXT("%f"), NormalImpulse.Length());
		if (CurrentGameMode)
		{
			ASoundManager* _SoundManager = CurrentGameMode->GetWorldSoundManager();
			if (_SoundManager && !SoundPlayed)
			{
				SoundPlayed = true;
				GetWorldTimerManager().SetTimer(SoundProtector, this, &APhysicsObjects::ProtectSound, 0.25f);
				
				FName _Tag1 = FName("DefaultSoundTag");
				if (Mesh->ComponentTags.Num() >= 3)
					_Tag1 = Mesh->ComponentTags[2]; // 3. Tag ses kategorisi tagidir 
				FName _Tag2 = FName("DefaultSoundTag");
				if (OtherComp->ComponentTags.Num() >= 3)
					_Tag2 = OtherComp->ComponentTags[2];
				_SoundManager->CalculateAndPlaySound(_Tag1, _Tag2, NormalImpulse, Hit.ImpactPoint);
				UE_LOG(LogTemp, Display, TEXT("Sound execution started"));
			}
		}
	}
}

void APhysicsObjects::ProtectSound()
{
	SoundPlayed = false;
}

FTransform APhysicsObjects::GetGrabPointSocketTransform()
{
	return FTransform::Identity;
}

