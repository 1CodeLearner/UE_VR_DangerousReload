// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../WidgetDisplayActor.h"
#include "WidgetDisplayActor_Turns.generated.h"

class UVRWidget_Turns;

/**
 *
 */
UCLASS()
class DANGEROUSRELOAD_API AWidgetDisplayActor_Turns : public AWidgetDisplayActor
{
	GENERATED_BODY()
public:
	virtual void DisplayUI(bool bEnabled) override;
protected:
	virtual void BeginPlay() override;
	UPROPERTY()
	UVRWidget_Turns* TurnsWidget;
};
