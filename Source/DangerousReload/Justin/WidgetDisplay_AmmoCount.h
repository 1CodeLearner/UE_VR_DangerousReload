// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetDisplayActor.h"
#include "WidgetDisplay_AmmoCount.generated.h"

/**
 *
 */
class UVRWidget_AmmoCount;
class AVRInteractableActor_Pistol;
UCLASS()
class DANGEROUSRELOAD_API AWidgetDisplay_AmmoCount : public AWidgetDisplayActor
{
	GENERATED_BODY()
public:
	virtual void Display(bool bEnabled) override; 
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UVRWidget_AmmoCount> AmmoCountWidget;

	UPROPERTY()
	TObjectPtr<AVRInteractableActor_Pistol> Weapon;
};
