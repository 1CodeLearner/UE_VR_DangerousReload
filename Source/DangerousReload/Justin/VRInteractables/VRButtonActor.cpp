// Fill out your copyright notice in the Description page of Project Settings.


#include "VRButtonActor.h"
#include "../../VRGameStateBase.h"
#include "Components/SkeletalMeshComponent.h"


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
}

void AVRButtonActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto GameState = GetWorld()->GetGameState<AVRGameStateBase>();

	if (GameState) {
		GameState->ChangeMatchStateTo(MatchStateToChangeTo);
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

