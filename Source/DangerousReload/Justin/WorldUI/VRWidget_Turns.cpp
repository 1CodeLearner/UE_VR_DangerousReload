// Fill out your copyright notice in the Description page of Project Settings.


#include "VRWidget_Turns.h"
#include "../../VRGameStateBase.h"
#include "Components/TextBlock.h"
#include "EngineUtils.h"
#include "../VRCharacter.h"

void UVRWidget_Turns::Display(bool bEnable)
{
	Super::Display(bEnable);

	auto VRGameState = GetWorld()->GetGameState<AVRGameStateBase>();
	

	AVRCharacter* Character = nullptr;
	for (TActorIterator<AVRCharacter> It(GetWorld()); It; ++It) 
	{
		Character = *It;
	}

	if (VRGameState && Character) 
	{
		FText Output = FText::FromString(FString::Printf(TEXT("%s"), VRGameState->IsCurrentTurn(Character) ? TEXT("Player") : TEXT("Dealer")));
		TurnsText->SetText(Output);
	}
}
