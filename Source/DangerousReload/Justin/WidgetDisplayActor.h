// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WidgetDisplayActor.generated.h"

class UWidgetComponent;

UCLASS()
class DANGEROUSRELOAD_API AWidgetDisplayActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AWidgetDisplayActor();

	virtual void Display(bool bEnabled);
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Settings")
	TObjectPtr<UWidgetComponent> WidgetComp;
};
