// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "VRGameStateBase.generated.h"

/**
 *
 */

UENUM()
enum class EMatchState : uint8
{
	EMATCH_Default UMETA(DisplayName = "Default"),
	EMATCH_Menu UMETA(Displayname = "Menu"),
	EMATCH_Start UMETA(DisplayName = "Start"),
	EMATCH_OnGoing UMETA(DisplayName = "OnGoing"),
	EMATCH_SwitchTurn UMETA(DisplayName = "SwitchTurn"),
	EMATCH_RoundReset UMETA(DisplayName = "RoundReset"),
	EMATCH_StageReset UMETA(DisplayName = "StageReset"),
	EMATCH_Stop UMETA(DisplayName = "Stop"),
};

UENUM()
enum class EGameState : uint8
{
	EGAME_Default UMETA(DisplayName = "Default"),
	EGAME_Menu UMETA(DisplayName = "GameMenu"),
	EGAME_Start UMETA(DisplayName = "GameStart"),
	EGAME_OnGoing UMETA(DisplayName = "GameOnGoing"),
	EGAME_Reset UMETA(DisplayName = "GameReset"),
	EGAME_Stop UMETA(DisplayName = "GameStop")
};


DECLARE_MULTICAST_DELEGATE_OneParam(FMatchStateChangedDelegate, EMatchState CurrentMatchstate);
DECLARE_MULTICAST_DELEGATE_OneParam(FGameStateChangedDelegate, EGameState CurrentGameState); //Not in use

UCLASS()
class DANGEROUSRELOAD_API AVRGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	AVRGameStateBase();

	virtual void Tick( float DeltaSeconds ) override;

	EGameState GameStateEnum;
	EMatchState MatchStateEnum;
	FMatchStateChangedDelegate OnMatchStateChanged;
	FGameStateChangedDelegate OnGameStateChanged;
	void ChangeGameStateTo(EGameState GameState);
	void ChangeMatchStateTo(EMatchState MatchState); //Not in use

	bool IsPlaying() const;
	int GetMatchCount() const;

	AActor* CurrentTurn;

private:
	bool bIsFirstTimePlaying;
	bool bIsPlaying;

	int MatchCount;

	FString GetBodyEnumAsString(EGameState value);
	FString GetBodyEnumAsString(EMatchState value);
};
