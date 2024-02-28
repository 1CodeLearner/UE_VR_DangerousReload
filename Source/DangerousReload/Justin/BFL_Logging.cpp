// Fill out your copyright notice in the Description page of Project Settings.


#include "BFL_Logging.h"
#include "VRCharacter.h"
#include "Components/TextRenderComponent.h"


void UBFL_Logging::Log(AActor* Player, FString Message, EHand HandEnum /* = EHand::LEFT*/)
{
	auto P = Cast<AVRCharacter>(Player);
	if (P)
	{
		switch (HandEnum)
		{
		case EHand::LEFT:
		{
			P->LTextComp->SetText(FText::FromString(FString::Printf(TEXT("%s"), *Message)));
			break;
		}
		case EHand::RIGHT:
		{
			P->RTextComp->SetText(FText::FromString(FString::Printf(TEXT("%s"), *Message)));
			break;
		}

		}
	}
}

void UBFL_Logging::GEngineLog(FString Message, FColor Color /*Magenta*/)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, Message, false);
}
