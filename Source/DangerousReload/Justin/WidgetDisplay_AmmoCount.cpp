// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetDisplay_AmmoCount.h"
#include "Components/WidgetComponent.h"
#include "VRWidget_AmmoCount.h"
#include "EngineUtils.h"
#include "VRInteractables//VRInteractableActor_Pistol.h"

void AWidgetDisplay_AmmoCount::DisplayUI(bool bEnabled)
{
	Super::DisplayUI(bEnabled);
	if (bEnabled)
	{
		if (ensure(Weapon))
		{
			int liveRounds = Weapon->GetLiveRounds();
			int blanks = Weapon->GetTotalRounds() - liveRounds;
			AmmoCountWidget->DisplayAmmoCount(true, liveRounds, blanks);
		}
	}

}

void AWidgetDisplay_AmmoCount::BeginPlay()
{
	Super::BeginPlay();
	if (WidgetComp->GetWidget())
	{
		auto widget = Cast<UVRWidget_AmmoCount>(WidgetComp->GetWidget());
		if (ensure(widget))
		{
			AmmoCountWidget = widget;
			AmmoCountWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	for (TActorIterator<AVRInteractableActor_Pistol> Iter(GetWorld()); Iter; ++Iter)
	{
		if (!Weapon)
		{
			Weapon = *Iter;
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Weapon %s"), *GetNameSafe(Weapon)), false);
}
