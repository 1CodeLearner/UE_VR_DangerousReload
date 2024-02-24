// Fill out your copyright notice in the Description page of Project Settings.


#include "DVRGameModeBase.h"
#include "Justin/VRCharacter.h"
#include "JINA/CSpotLight.h"

ADVRGameModeBase::ADVRGameModeBase()
{

}

void ADVRGameModeBase::ChangeLifeLightColor(ACharacter* target, FLinearColor color)
{
	if (Cast<AVRCharacter>(target) != nullptr) {
		for (int32 i = 0; i < playerLifeSpotlight.Num(); i++) {
			if (playerLifeSpotlight[i]->GetLightColor() == color) continue;
			else
			{
				playerLifeSpotlight[i]->SetLightColor(color);
				break;
			}
		}
	}
	else
	{
		for (int32 i = 0; i < enemyLifeSpotlight.Num(); i++) {
			if(enemyLifeSpotlight[i]->GetLightColor() == color) continue;
			else 
			{
				enemyLifeSpotlight[i]->SetLightColor(color);
				break;
			}
		}
	}
}
