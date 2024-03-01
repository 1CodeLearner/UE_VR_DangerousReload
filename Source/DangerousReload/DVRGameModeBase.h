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

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;

protected:
	void OnMatchStateChanged(EMatchState CurrentMatchState);

	void StartMatch();

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

	UFUNCTION()
	void SwitchTurns();
	UFUNCTION()
	void RestartMatch();

	UPROPERTY()
	TObjectPtr<AVRGameStateBase> VRGameState;

	FTransform PistolRespawnTransform;

};
