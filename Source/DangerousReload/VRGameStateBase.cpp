// Fill out your copyright notice in the Description page of Project Settings.


#include "VRGameStateBase.h"
#include "DVRGameModeBase.h"

AVRGameStateBase::AVRGameStateBase()
{
	GameStateEnum = EGameState::EGAME_Default;
	MatchStateEnum = EMatchState::EMATCH_Default;
	bMatchOver = true;
	CurrentTurn = nullptr;
	bIsFirstTimePlaying = true;
}

void AVRGameStateBase::ChangeGameStateTo(EGameState GameState)
{
	GameStateEnum = GameState;
	OnGameStateChanged.Broadcast();
}

void AVRGameStateBase::ChangeMatchStateTo(EMatchState MatchState)
{
	MatchStateEnum = MatchState;
	OnMatchStateChanged.Broadcast();
}

