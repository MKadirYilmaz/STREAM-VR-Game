// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingSystem/PaintComponent.h"
#include "PaintingSystem/WritableActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMaterialLibrary.h"

UPaintComponent::UPaintComponent()
{
}

void UPaintComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPaintComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
