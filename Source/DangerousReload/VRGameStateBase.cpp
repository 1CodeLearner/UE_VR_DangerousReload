// Fill out your copyright notice in the Description page of Project Settings.


#include "VRGameStateBase.h"
#include "Justin/BFL_Logging.h"

AVRGameStateBase::AVRGameStateBase()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	bIsPlaying = false;
	bIsFirstTimePlaying = true;

	MatchCount = 0;
	CurrentTurn = nullptr;

	MatchStateEnum = EMatchState::EMATCH_Default;
}

void AVRGameStateBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	auto VRCharacter = Cast<AActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (ensure(VRCharacter))
	{
		//UBFL_Logging::Log(VRCharacter, FString::Printf(TEXT("GameState: %s"), *GetBodyEnumAsString(GameStateEnum)), EHand::LEFT);

		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Emerald, FString::Printf(TEXT("CurrentTurn: %s"), *GetNameSafe(CurrentTurn)));
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Emerald, FString::Printf(TEXT("MatchState: %s"), *GetBodyEnumAsString(MatchStateEnum)));

		UBFL_Logging::Log(VRCharacter, FString::Printf(TEXT("CurrentTurn: %s"), *GetNameSafe(CurrentTurn)));
		UBFL_Logging::Log(VRCharacter, FString::Printf(TEXT("MatchState: %s"), *GetBodyEnumAsString(MatchStateEnum)), EHand::RIGHT);
	}
}


bool AVRGameStateBase::IsMatchState(EMatchState CheckMatchState) const
{
	return MatchStateEnum == CheckMatchState;
}

bool AVRGameStateBase::IsCurrentTurn(AActor* ActorToCheck) const
{
	return CurrentTurn == ActorToCheck;
}

/*AActor* AVRGameStateBase::GetCurrentTurn() const
{
	if (CurrentTurn) return CurrentTurn;
	return nullptr;
}*/

void AVRGameStateBase::ChangeMatchStateTo(EMatchState MatchState)
{
	MatchStateEnum = MatchState;
	OnMatchStateChanged.Broadcast(MatchStateEnum);
}

void AVRGameStateBase::SetCurrentTurn(AActor* ActorTurn)
{
	this->CurrentTurn = ActorTurn;
}

bool AVRGameStateBase::IsPlaying() const
{
	return bIsPlaying;
}

int AVRGameStateBase::GetMatchCount() const
{
	return MatchCount;
}

FString AVRGameStateBase::GetBodyEnumAsString(EMatchState value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EMatchState"), true);
	if (!EnumPtr) return FString("Invalid");

	return EnumPtr->GetNameStringByIndex((int32)value);
}