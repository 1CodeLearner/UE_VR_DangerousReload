// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DVRGameModeBase.generated.h"

/**
 *
 */


class AVRCharacter;
class ACEnemy;
class AVRInteractableActor;
class AVRInteractableActor_Pistol;
class AVRGameStateBase;

USTRUCT(BlueprintType)
struct FRound : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int Health;
};


class UDataTable;

UCLASS()
class DANGEROUSRELOAD_API ADVRGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	bool isPlayerTurn = false;
	int bulletCount;

	void StartMatch();

	void OnFired(AActor* ActorInstigator, AActor* ActorHit, bool bIsLiveRound);
	

	bool IsMatchOver() const;

	UPROPERTY(EditAnywhere, Category = "My Settings")
	TArray<class ACSpotLightActor*> playerLifeSpotlight;

	UPROPERTY(EditAnywhere, Category = "My Settings")
	TArray<class ACSpotLightActor*> enemyLifeSpotlight;


public:
	ADVRGameModeBase();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	void ChangeLifeLightColor(ACharacter* target, FLinearColor color);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<UDataTable> DT_Matches;

	void RespawnPistol();

private:
	UPROPERTY()
	TObjectPtr<AVRCharacter> Player;
	UPROPERTY()
	TObjectPtr<ACEnemy> Enemy;
	UPROPERTY()
	TObjectPtr<AVRInteractableActor_Pistol> Pistol;
	UPROPERTY(EditDefaultsOnly, Category = "My Settings")
	TArray<TSubclassOf<AVRInteractableActor>> ItemClasses;

	UPROPERTY()
	TObjectPtr<AVRGameStateBase> VRGameState;
};
