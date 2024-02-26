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
	EMATCH_Default UMETA(DisplayName="Default"),
	EMATCH_Start UMETA(DisplayName="MatchStart"),
	EMATCH_OnGoing UMETA(DisplayName="MatchOnGoing"),
	EMATCH_SwitchTurn UMETA(DisplayName="MatchSwitchTurn"),
	EMATCH_Reset UMETA(DisplayName="MatchReset"),
	EMATCH_Stop UMETA(DisplayName = "MatchStop"),
};

UENUM()
enum class EGameState : uint8
{
	EGAME_Default UMETA(DisplayName="Default"),
	EGAME_Menu UMETA(DisplayName="GameMenu"),
	EGAME_Start UMETA(DisplayName="GameStart"),
	EGAME_OnGoing UMETA(DisplayName="GameOnGoing"),
	EGAME_Stop UMETA(DisplayName="GameStop")
};


DECLARE_MULTICAST_DELEGATE(FMatchStateChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FGameStateChangedDelegate);

UCLASS()
class DANGEROUSRELOAD_API AVRGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	AVRGameStateBase();

	FMatchStateChangedDelegate OnMatchStateChanged;
	FGameStateChangedDelegate OnGameStateChanged;

	bool bIsFirstTimePlaying;
	bool bMatchOver;
	AActor* CurrentTurn;

	EGameState GameStateEnum; 
	EMatchState MatchStateEnum;

	void ChangeGameStateTo(EGameState GameState);
	void ChangeMatchStateTo(EMatchState MatchState);
};
