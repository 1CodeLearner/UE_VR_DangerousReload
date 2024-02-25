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

	UPROPERTY(EditAnywhere, Category = "My Settings")
	TArray<class ACSpotLightActor*> playerLifeSpotlight;

	UPROPERTY(EditAnywhere, Category = "My Settings")
	TArray<class ACSpotLightActor*> enemyLifeSpotlight;
	

public:
	ADVRGameModeBase();
	void ChangeLifeLightColor(ACharacter* target, FLinearColor color);
};
