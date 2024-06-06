// All rights belong to Kadir Yilmaz


#include "MathMiniGame/MathLaserShooter.h"
#include "MathMiniGame/MathBomb.h"

// Sets default values
AMathLaserShooter::AMathLaserShooter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LaserShooterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserShooterMesh"));
	LaserBeamMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserBeamMesh"));

	RootComponent = LaserShooterMesh;
	LaserBeamMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMathLaserShooter::BeginPlay()
{
	Super::BeginPlay();
	
	DefaultRotation = LaserShooterMesh->GetRelativeRotation();
}

// Called every frame
void AMathLaserShooter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShouldTrace && TargetBomb)
	{
		LaserShooterMesh->SetWorldRotation(FMath::RInterpTo(LaserShooterMesh->GetRelativeRotation(),
			(TargetBomb->GetActorLocation() - LaserShooterMesh->GetComponentLocation()).Rotation(), DeltaTime, 3.f)); // Laser mesh will trace the selected bomb mesh
	}
	else
	{
		LaserShooterMesh->SetRelativeRotation(FMath::RInterpTo(LaserShooterMesh->GetRelativeRotation(), DefaultRotation, DeltaTime, 3.f));
	}
}

void AMathLaserShooter::SetFreeLaser()
{
	ShouldTrace = false;
	TargetBomb = nullptr;
}

