// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../VRWidget.h"
#include "VRWidget_ButtonPress.generated.h"

/**
 *
 */
class UTextBlock;

UCLASS()
class DANGEROUSRELOAD_API UVRWidget_ButtonPress : public UVRWidget
{
	GENERATED_BODY()
public:
	void DisplayButton(bool bEnabled);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Settings")
	UTextBlock* TextBlockDisplay;
};
