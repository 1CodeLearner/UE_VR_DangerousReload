// Fill out your copyright notice in the Description page of Project Settings.


#include "DVRGameModeBase.h"
#include "Justin/VRCharacter.h"/*
#include "JINA/CSpotLight.h"*/
#include "Justin/VRHealthComponent.h"

ADVRGameModeBase::ADVRGameModeBase()
{

}


void ADVRGameModeBase::StartRound()
{

}

void ADVRGameModeBase::OnFired(AActor* ActorHit)
{
	ACharacter* CharacterHit = Cast<ACharacter>(ActorHit);
	if (ensure(CharacterHit))
	{
		UVRHealthComponent* HealthComp = CharacterHit->GetComponentByClass<UVRHealthComponent>();
		if (ensure(HealthComp))
		{
			HealthComp->InflictDamage();
			ChangeLifeLightColor(CharacterHit, FLinearColor(0, 0, 0, 0));
		}
	}
}

void ADVRGameModeBase::ChangeLifeLightColor(ACharacter* target, FLinearColor color)
{
	/*if (Cast<AVRCharacter>(target) != nullptr) {
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
	}*/
}
