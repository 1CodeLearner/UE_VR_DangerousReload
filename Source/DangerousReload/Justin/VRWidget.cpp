// Fill out your copyright notice in the Description page of Project Settings.


#include "VRWidget.h"

void UVRWidget::Display(bool bEnabled)
{
	if (bEnabled)
		SetVisibility(ESlateVisibility::Visible);
	else
		SetVisibility(ESlateVisibility::Collapsed);
}
