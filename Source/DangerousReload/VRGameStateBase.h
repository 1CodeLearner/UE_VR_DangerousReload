// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "VRGameStateBase.generated.h"


class DVRGameModeBase;

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

/*UENUM()
enum class EGameState : uint8
{
	EGAME_Default UMETA(DisplayName = "Default"),
	EGAME_Menu UMETA(DisplayName = "GameMenu"),
	EGAME_Start UMETA(DisplayName = "GameStart"),
	EGAME_OnGoing UMETA(DisplayName = "GameOnGoing"),
	EGAME_Reset UMETA(DisplayName = "GameReset"),
	EGAME_Stop UMETA(DisplayName = "GameStop")
};*/


DECLARE_MULTICAST_DELEGATE_OneParam(FMatchStateChangedDelegate, EMatchState CurrentMatchstate);

UCLASS()
class DANGEROUSRELOAD_API AVRGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	AVRGameStateBase();

	virtual void Tick( float DeltaSeconds ) override;

	FMatchStateChangedDelegate OnMatchStateChanged;
	bool IsMatchState(EMatchState CheckMatchState) const;
	AActor* GetCurrentTurn() const;

	bool IsPlaying() const;
	int GetMatchCount() const;


private:
	
	/*Only these classes can change the state of the game:*/
	friend class ADVRGameModeBase;
	friend class AVRButtonActor;

	void ChangeMatchStateTo(EMatchState MatchState);
	void SetCurrentTurn(AActor* ActorTurn);
	bool bIsFirstTimePlaying;
	bool bIsPlaying;

	UPROPERTY()
	AActor* CurrentTurn;

	int MatchCount;
	EMatchState MatchStateEnum;
	
	FString GetBodyEnumAsString(EMatchState value);
};