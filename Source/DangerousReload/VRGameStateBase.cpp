// Fill out your copyright notice in the Description page of Project Settings.


#include "VRGameStateBase.h"
#include "DVRGameModeBase.h"
#include "Justin/BFL_Logging.h"

AVRGameStateBase::AVRGameStateBase()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	bIsPlaying = false;
	bIsFirstTimePlaying = true;

	MatchCount = 0;
	CurrentTurn = nullptr;

	GameStateEnum = EGameState::EGAME_Default;
	MatchStateEnum = EMatchState::EMATCH_Default;
}

void AVRGameStateBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	auto VRCharacter = Cast<AActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (ensure(VRCharacter))
	{
		//UBFL_Logging::Log(VRCharacter, FString::Printf(TEXT("GameState: %s"), *GetBodyEnumAsString(GameStateEnum)), EHand::LEFT);

		UBFL_Logging::Log(VRCharacter, FString::Printf(TEXT("CurrentTurn: %s"), *GetNameSafe(CurrentTurn)));
		UBFL_Logging::Log(VRCharacter, FString::Printf(TEXT("MatchState: %s"), *GetBodyEnumAsString(MatchStateEnum)), EHand::RIGHT);
	}
}

void AVRGameStateBase::ChangeGameStateTo(EGameState GameState)
{
	GameStateEnum = GameState;
	if (GameStateEnum == EGameState::EGAME_Start)
	{
		bIsPlaying = true;
	}
	OnGameStateChanged.Broadcast(GameStateEnum);
}

void AVRGameStateBase::ChangeMatchStateTo(EMatchState MatchState)
{
	MatchStateEnum = MatchState;
	OnMatchStateChanged.Broadcast(MatchStateEnum);
}

bool AVRGameStateBase::IsPlaying() const
{
	return bIsPlaying;
}

int AVRGameStateBase::GetMatchCount() const
{
	return MatchCount;
}

FString AVRGameStateBase::GetBodyEnumAsString(EGameState value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EGameState"), true);
	if (!EnumPtr) return FString("Invalid");

	return EnumPtr->GetNameStringByIndex((int32)value);
}

FString AVRGameStateBase::GetBodyEnumAsString(EMatchState value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EMatchState"), true);
	if (!EnumPtr) return FString("Invalid");

	return EnumPtr->GetNameStringByIndex((int32)value);
}