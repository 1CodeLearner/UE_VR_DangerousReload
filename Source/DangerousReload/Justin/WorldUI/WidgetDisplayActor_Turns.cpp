// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetDisplayActor_Turns.h"
#include "VRWidget_Turns.h"
#include "Components/WidgetComponent.h"

void AWidgetDisplayActor_Turns::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetComp->GetWidget())
	{
		auto widget = Cast<UVRWidget_Turns>(WidgetComp->GetWidget());
		if (ensure(widget))
		{
			TurnsWidget = widget;
			TurnsWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}


void AWidgetDisplayActor_Turns::DisplayUI(bool bEnabled) 
{
	Super::DisplayUI(bEnabled);

	TurnsWidget->Display(bEnabled);
}