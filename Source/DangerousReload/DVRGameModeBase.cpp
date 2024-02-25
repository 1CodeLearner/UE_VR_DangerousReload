// Fill out your copyright notice in the Description page of Project Settings.


#include "DVRGameModeBase.h"
#include "Justin/VRCharacter.h"/*
#include "JINA/CSpotLight.h"*/
#include "Justin/VRHealthComponent.h"
#include "Justin/VRCharacter.h"
#include "JINA/CEnemy.h"
#include "EngineUtils.h"
#include "JINA/CSpotLightActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SpotLightComponent.h"


ADVRGameModeBase::ADVRGameModeBase()
{
	MatchCount = 0;
	bCanFire = false;
}

void ADVRGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	for (TActorIterator<AVRCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		if (!Player)
		{
			Player = *Iter;
		}
	}

	for (TActorIterator<ACEnemy> Iter(GetWorld()); Iter; ++Iter)
	{
		if (!Enemy)
		{
			Enemy = *Iter;
		}
	}
}

void ADVRGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	StartMatch();
}

void ADVRGameModeBase::StartMatch()
{
	if (ensure(DT_Rounds))
	{
		TArray<FName> Names = DT_Rounds->GetRowNames();
		const TCHAR* Context = TEXT("Context");
		FRound* Round = DT_Rounds->FindRow<FRound>(Names[MatchCount], Context);
		MatchCount++;

		if (Player)
		{
			auto PlayerHealth = Player->GetComponentByClass<UVRHealthComponent>();
			if (ensure(PlayerHealth))
			{
				PlayerHealth->SetMaxHealth(Round->Health);
			}
		}
		if (Enemy)
		{
			auto EnemyHealth = Enemy->GetComponentByClass<UVRHealthComponent>();

			if (ensure(EnemyHealth))
			{
				EnemyHealth->SetMaxHealth(Round->Health);
			}
		}
	}

	isPlayerTurn = true;
	bCanFire = true;
	OnMatchStart.Broadcast();
}


void ADVRGameModeBase::OnFired(AActor* ActorInstigator, AActor* ActorAimed, bool bIsLiveRound)
{
	if (bIsLiveRound)
	{
		UVRHealthComponent* HealthComp = ActorAimed->GetComponentByClass<UVRHealthComponent>();
		if (ensure(HealthComp))
			HealthComp->InflictDamage();

		ACharacter* CharacterHit = Cast<ACharacter>(ActorAimed);
		if (CharacterHit)
		{
			ChangeLifeLightColor(CharacterHit, FLinearColor(0, 0, 0, 0));
		}

		//Switch Turns regardless who was shot
		if (ActorInstigator == Player)
		{
			CurrentTurn = Enemy;
		}
		else
		{
			CurrentTurn = Player;
		}
	}
	else
	{
		//Switch turn only if the blank was shot at other participant
		if (CurrentTurn != ActorAimed)
		{
			CurrentTurn = ActorAimed;
		}
		UGameplayStatics::PlaySoundAtLocation(Player, EmptyGunSound, Player->GetActorLocation(), FRotator::ZeroRotator);
	}

	bCanFire = false;
}

bool ADVRGameModeBase::CanFire() const
{
	return bCanFire;
}

void ADVRGameModeBase::SetCanFire(bool _bCanFire)
{
	bCanFire = _bCanFire;
	if(ensure(RackingSound))
	{
		UGameplayStatics::PlaySoundAtLocation(Player, RackingSound, Player->GetActorLocation(), FRotator::ZeroRotator);
	}
}

void ADVRGameModeBase::ChangeLifeLightColor(ACharacter* target, FLinearColor color)
{
	if (Cast<AVRCharacter>(target) != nullptr) {
		for (int32 i = 0; i < playerLifeSpotlight.Num(); i++) {
			if (playerLifeSpotlight[i]->spotLight->GetLightColor() == color) continue;
			else
			{
				playerLifeSpotlight[i]->spotLight->SetLightColor(color);
				break;
			}
		}
	}
	else
	{
		for (int32 i = 0; i < enemyLifeSpotlight.Num(); i++) {
			if(enemyLifeSpotlight[i]->spotLight->GetLightColor() == color) continue;
			else
			{
				enemyLifeSpotlight[i]->spotLight->SetLightColor(color);
				break;
			}
		}
	}
}
