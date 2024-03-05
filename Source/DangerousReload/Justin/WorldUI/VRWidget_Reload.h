// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../VRWidget.h"
#include "VRWidget_Reload.generated.h"

/**
 *
 */
UCLASS()
class DANGEROUSRELOAD_API UVRWidget_Reload : public UVRWidget
{
	GENERATED_BODY()
public:
	virtual void Display(bool bEnabled) override;
};
