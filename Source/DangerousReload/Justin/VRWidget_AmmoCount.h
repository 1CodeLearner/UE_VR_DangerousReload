// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRWidget.h"
#include "VRWidget_AmmoCount.generated.h"

/**
 *
 */
class UTextBlock;

UCLASS()
class DANGEROUSRELOAD_API UVRWidget_AmmoCount : public UVRWidget
{
	GENERATED_BODY()

public:
	void DisplayAmmoCount(bool bEnabled, int LiveRounds, int BlankRounds);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget), Category = "Settings")
	TObjectPtr<UTextBlock> LiveRoundsText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget), Category = "Settings")
	TObjectPtr<UTextBlock> BlankRoundsText;
};
