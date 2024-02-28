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

UCLASS()
class DANGEROUSRELOAD_API AVRGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	AVRGameStateBase();

	virtual void Tick( float DeltaSeconds ) override;

	FMatchStateChangedDelegate OnMatchStateChanged;
	void ChangeMatchStateTo(EMatchState MatchState);
	bool IsMatchState(EMatchState CheckMatchState);
	AActor* GetCurrentTurn() const;
	void SetCurrentTurn(AActor* ActorTurn);

	bool IsPlaying() const;
	int GetMatchCount() const;


private:
	bool bIsFirstTimePlaying;
	bool bIsPlaying;

	AActor* CurrentTurn;

	int MatchCount;
	EMatchState MatchStateEnum;
	
	FString GetBodyEnumAsString(EMatchState value);
};
