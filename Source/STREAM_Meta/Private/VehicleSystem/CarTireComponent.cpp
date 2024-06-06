// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleSystem/CarTireComponent.h"

UCarTireComponent::UCarTireComponent()
{

}

void UCarTireComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentTireDirection = GetForwardVector();
}
