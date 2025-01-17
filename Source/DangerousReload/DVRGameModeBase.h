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
class AWidgetDisplay_AmmoCount;
class AWidgetDisplayActor_ButtonPress;
class AWidgetDisplayActor_Turns;

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

public:

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

	bool bIsInitialized;
	void InitializeParticipants();

	//Game flow control
	void OnMatchStateChanged(EMatchState CurrentMatchState);

	//Changes from Main state to Start state
	friend class AVRButtonActor;

	//Invokes PreSwitchingState after certain duration
	void SwitchStateOnTimer(EMatchState MatchState, float InRate);
	//Setups before switching Match State
	void PreSwitchingState(EMatchState MatchState);

	//Weapon events: changes Match State
	void OnPickedUp();
	void OnDropped();
	void OnFired(AActor* ActorInstigator, AActor* ActorHit, bool bIsLiveRound);

	//UI
	void DisplayButtonPress(bool bEnable);
	TObjectPtr<AWidgetDisplayActor_ButtonPress> ButtonPressWidgetActor;
	void DisplayAmmoCount(bool bEnable);
	TObjectPtr<AWidgetDisplay_AmmoCount> AmmoCountWidgetActor;
	void DisplayTurnAndPickup(bool bEnable);
	TObjectPtr<AWidgetDisplayActor_Turns> TurnsWidgetActor;
	void DisplaySettingDownWeapon(bool bEnable);
	void DisplayRackingWeapon(bool bEnable);

	bool IsFinalStage() const;

	FTransform PistolRespawnTransform;
	FTimerHandle RespawnHandle;

};
