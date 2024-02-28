// Fill out your copyright notice in the Description page of Project Settings.


#include "VRButtonActor.h"
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
}

void AVRButtonActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto GameState = GetWorld()->GetGameState<AVRGameStateBase>();

	if (GameState) {
		GameState->ChangeMatchStateTo(MatchStateToChangeTo);
	}
}

