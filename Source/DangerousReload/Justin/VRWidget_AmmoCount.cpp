// Fill out your copyright notice in the Description page of Project Settings.


#include "VRWidget_AmmoCount.h"
#include "Components/TextBlock.h"

void UVRWidget_AmmoCount::DisplayAmmoCount(bool bEnabled, int LiveRounds, int BlankRounds)
{
	Display(bEnabled);

	LiveRoundsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), LiveRounds)));
	BlankRoundsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), BlankRounds)));
}
