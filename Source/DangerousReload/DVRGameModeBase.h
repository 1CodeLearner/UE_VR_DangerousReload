// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DVRGameModeBase.generated.h"

/**
 *
 */

DECLARE_MULTICAST_DELEGATE(FMatchStartDelegate);

class AVRCharacter;
class ACEnemy;

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
	FMatchStartDelegate OnMatchStart; 

	bool isPlayerTurn = false;
	int bulletCount;

	void StartMatch();

	void OnFired(AActor* ActorInstigator, AActor* ActorHit, bool bIsLiveRound);

	bool CanFire() const;
	void SetCanFire(bool _bCanFire);

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
	TObjectPtr<UDataTable> DT_Rounds;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<USoundBase> RackingSound;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<USoundBase> EmptyGunSound;

private:
	int MatchCount;
	bool bCanFire;

	TObjectPtr<AVRCharacter> Player;
	TObjectPtr<ACEnemy> Enemy;
	AActor* CurrentTurn;
};
