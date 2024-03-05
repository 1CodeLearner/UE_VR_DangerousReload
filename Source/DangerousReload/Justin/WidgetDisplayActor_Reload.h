// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/WidgetDisplayActor.h"
#include "WidgetDisplayActor_Reload.generated.h"

/**
 *
 */
UCLASS()
class DANGEROUSRELOAD_API AWidgetDisplayActor_Reload : public AWidgetDisplayActor
{
	GENERATED_BODY()
public:
	virtual void DisplayUI(bool bEnabled) override;
};
