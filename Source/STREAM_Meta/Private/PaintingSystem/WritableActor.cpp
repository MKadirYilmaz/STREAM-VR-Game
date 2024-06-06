// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingSystem/WritableActor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMaterialLibrary.h"

// Sets default values
AWritableActor::AWritableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WritableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WritableMesh"));

	RootComponent = WritableMesh;
}

// Called when the game starts or when spawned
void AWritableActor::BeginPlay()
{
	Super::BeginPlay();
	SetupMaterialsInBlueprint(); // Butun gerekli materyalleri blueprint tarafinda hazirlar

	PenMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, PenMaterialBase);
	BoardMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, BoardMaterialBase);
	CreateCustomRenderTargets2D();
	//AssignRenderTargets();
	//BoardMaterial->SetTextureParameterValue(FName("RenderTarget"), RenderTarget2D);
	WritableMesh->SetMaterial(0, BoardMaterial);
}

// Called every frame
void AWritableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*

void AWritableActor::AssignRenderTargets()
{
	for (int32 i = 0; i < TextureRenderTargets.Num(); i++)
	{
		if (i < RenderTargetParamNames.Num())
		{
			BoardMaterial->SetTextureParameterValue(RenderTargetParamNames[i], TextureRenderTargets[i]);
			UE_LOG(LogTemp, Display, TEXT("%d index render target has been assigned"), i);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("There is no equivelent parameter name for this RenderTarget2D (index: %d)"), i);
		}
	}
}

UTextureRenderTarget2D* AWritableActor::GetCorrectRenderTarget2D(int32 Index)
{
	if (Index >= TextureRenderTargets.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Given index is out of bound"));
		return nullptr;
	}
	return TextureRenderTargets[Index];
}
*/

