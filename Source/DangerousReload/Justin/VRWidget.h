// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VRWidget.generated.h"

/**
 * 
 */
UCLASS()
class DANGEROUSRELOAD_API UVRWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Display(bool bEnabled);
};
