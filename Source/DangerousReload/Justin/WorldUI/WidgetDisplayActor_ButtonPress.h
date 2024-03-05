// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../WidgetDisplayActor.h"
#include "WidgetDisplayActor_ButtonPress.generated.h"

/**
 *
 */
 class UVRWidget_ButtonPress;

UCLASS()
class DANGEROUSRELOAD_API AWidgetDisplayActor_ButtonPress : public AWidgetDisplayActor
{
	GENERATED_BODY()
public:
	virtual void DisplayUI(bool bEnabled) override;
protected:
	virtual void BeginPlay() override;
	UPROPERTY()
	TObjectPtr<UVRWidget_ButtonPress> ButtonPressWidget;
};
