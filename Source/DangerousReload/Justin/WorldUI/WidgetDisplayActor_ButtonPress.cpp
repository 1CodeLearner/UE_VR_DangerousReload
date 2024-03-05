// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetDisplayActor_ButtonPress.h"
#include "Components/WidgetComponent.h"
#include "VRWidget_ButtonPress.h"

void AWidgetDisplayActor_ButtonPress::DisplayUI(bool bEnabled)
{
	Super::DisplayUI(bEnabled);

	if (bEnabled)
	{
		ButtonPressWidget->DisplayButton(bEnabled);
	}
}

void AWidgetDisplayActor_ButtonPress::BeginPlay()
{
	Super::BeginPlay();
	
	if (WidgetComp->GetWidget())
	{
		auto widget = Cast<UVRWidget_ButtonPress>(WidgetComp->GetWidget());
		if (ensure(widget))
		{
			ButtonPressWidget = widget;
		}
	}
}
