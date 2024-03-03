// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DVRGameModeBase.generated.h"

/**
 *
 */


enum class EMatchState : uint8;
class AVRGameStateBase;
class AVRCharacter;
class ACEnemy;
class AVRInteractableActor;
class AVRInteractableActor_Pistol;

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
	ADVRGameModeBase();

	//***For enemy Testing only***
	bool isPlayerTurn = false;

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;

protected:
	void OnMatchStateChanged(EMatchState CurrentMatchState);

public:
	void OnFired(AActor* ActorInstigator, AActor* ActorHit, bool bIsLiveRound);

	int bulletCount;

	UPROPERTY(EditAnywhere, Category = "My Settings")
	TArray<class ACSpotLightActor*> playerLifeSpotlight;

	UPROPERTY(EditAnywhere, Category = "My Settings")
	TArray<class ACSpotLightActor*> enemyLifeSpotlight;

	void ChangeLifeLightColor(ACharacter* target, FLinearColor color);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<UDataTable> DT_Matches;


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

	FTransform PistolRespawnTransform;

	
	void RespawnPistol();
	UFUNCTION()
	void RestartMatch();

	void Menu();
	void Start();
	void OnGoing();
	void Stop();
	void SwitchTurns();
	void RoundReset();
	void StageClear();
	void StageLost();
	void GameClear();
	void GameOver();

};
