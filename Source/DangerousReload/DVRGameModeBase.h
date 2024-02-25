// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DVRGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DANGEROUSRELOAD_API ADVRGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	bool isPlayerTurn = false;
	float bulletCount;

	void StartRound();
	void OnFired(AActor* ActorHit);
	/*UPROPERTY(EditAnywhere, Category = "My Settings")
	TArray<class ACSpotLight*> playerLifeSpotlight;

	UPROPERTY(EditAnywhere, Category = "My Settings")
	TArray<class ACSpotLight*> enemyLifeSpotlight;*/
	

public:
	ADVRGameModeBase();
	void ChangeLifeLightColor(ACharacter* target, FLinearColor color);
};
