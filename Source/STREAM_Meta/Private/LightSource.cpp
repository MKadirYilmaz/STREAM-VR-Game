// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSource.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

// Sets default values
ALightSource::ALightSource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LightSourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightSourceMesh"));
	RootComponent = LightSourceMesh;

}

// Called when the game starts or when spawned
void ALightSource::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALightSource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALightSource::ChangeLightCondition(bool bCondition)
{
	if (!LSMDynamic || !LightComp)
	{
		UE_LOG(LogTemp, Error, TEXT("LSMDynamic or LightComp is nullptr! LightSource/ChangeLightCondition"));
		return;
	}
	if (bCondition)
	{	
		LSMDynamic->SetScalarParameterValue(FName("LightEmissive"), EmissivePower);
		LightComp->SetIntensity(InitialIntensity);
	}
	else
	{
		LSMDynamic->SetScalarParameterValue(FName("LightEmissive"), 0);
		LightComp->SetIntensity(0);
	}
	LightComp->SetActive(bCondition);
	LCondition = bCondition;
}

void ALightSource::SetStartCondition(bool StartCondition)
{
	if (UsesSpotLight)
	{
		USpotLightComponent* _SpotLight = GetComponentByClass<USpotLightComponent>();
		LightComp = _SpotLight;
	}
	else
	{
		UPointLightComponent* _PointLight = GetComponentByClass<UPointLightComponent>();
		LightComp = _PointLight;
	}

	LSMDynamic = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, LightSourceMaterial);
	if (!LSMDynamic || !LightComp)
	{
		UE_LOG(LogTemp, Error, TEXT("LSMDynamic or LightComp is nullptr! LightSource/SetStartCondition"));
		return;
	}
	LSMDynamic->SetVectorParameterValue(FName("LightColor"), LightComp->GetLightColor()); // Lamba meshinin yanacagi rengi ayarlar

	LCondition = StartCondition;
	InitialIntensity = LightComp->Intensity;
	float _StartIntensity;
	if (LCondition)
		_StartIntensity = InitialIntensity;
	else
		_StartIntensity = 0;

	LSMDynamic->SetScalarParameterValue(FName("LightEmissive"), FMath::Clamp(_StartIntensity, 0, EmissivePower)); // Lamba mashinin parlayacagi isik siddetini ayarlar

	LightComp->SetIntensity(_StartIntensity);
	LightComp->SetActive(LCondition);

	LightSourceMesh->SetMaterial(1, LSMDynamic);
}

