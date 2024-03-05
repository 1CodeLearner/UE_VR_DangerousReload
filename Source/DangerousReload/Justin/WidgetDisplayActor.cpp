// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetDisplayActor.h"
#include "Components/WidgetComponent.h"

AWidgetDisplayActor::AWidgetDisplayActor()
{
	PrimaryActorTick.bCanEverTick = true;
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComp->SetupAttachment(RootComponent);
}

void AWidgetDisplayActor::DisplayUI(bool bEnabled)
{
	auto widget = WidgetComp->GetWidget();
	if (widget)
	{
		if (bEnabled)
		{
			widget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			widget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void AWidgetDisplayActor::BeginPlay()
{
	Super::BeginPlay();

}

