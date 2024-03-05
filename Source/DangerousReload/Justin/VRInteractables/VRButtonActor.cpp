// Fill out your copyright notice in the Description page of Project Settings.


#include "VRButtonActor.h"
#include "../../VRGameStateBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "../../DVRGameModeBase.h"


AVRButtonActor::AVRButtonActor()
{
	PrimaryActorTick.bCanEverTick = true;
	ButtonMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("ButtonMeshComp");
	ButtonMeshComp->SetupAttachment(RootComponent);
}

void AVRButtonActor::BeginPlay()
{
	Super::BeginPlay();
	ButtonMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AVRButtonActor::OnBeginOverlap);

	VRGameState = GetWorld()->GetGameState<AVRGameStateBase>();
	if (ensure(VRGameState))
	{
		VRGameState->OnMatchStateChanged.AddUObject(this, &AVRButtonActor::OnMatchStateChanged);
	}

	VRGameMode = GetWorld()->GetAuthGameMode<ADVRGameModeBase>();
}

void AVRButtonActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto GameState = GetWorld()->GetGameState<AVRGameStateBase>();

	if (GameState) {
		VRGameMode->PreSwitchingState(EMatchState::EMATCH_Start);
		ButtonMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AVRButtonActor::OnMatchStateChanged(EMatchState MatchState)
{
	switch (MatchState)
	{
	case EMatchState::EMATCH_GameClear:
	{
		ButtonMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;
	}
	case EMatchState::EMATCH_GameOver:
	{
		ButtonMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;
	}
	}
}

