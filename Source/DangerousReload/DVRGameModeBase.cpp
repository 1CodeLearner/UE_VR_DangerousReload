// Fill out your copyright notice in the Description page of Project Settings.


#include "DVRGameModeBase.h"
#include "Justin/VRCharacter.h"
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
	bMatchOver = false;
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

	TArray<ACSpotLightActor*> lifeLight;
	for (TObjectIterator<ACSpotLightActor> it; it; ++it) {
		lifeLight.Add(*it);
	}
	for (int32 i = 0; i < 4; ++i)
	{
		playerLifeSpotlight.Add(Cast<ACSpotLightActor>(lifeLight[i]));
	}
	for (int32 i = 0; i < 4; ++i)
	{
		enemyLifeSpotlight.Add(Cast<ACSpotLightActor>(lifeLight[i + 4]));
	}
}

void ADVRGameModeBase::StartMatch()
{
	if (ensure(DT_Matches))
	{
		TArray<FName> Names = DT_Matches->GetRowNames();
		const TCHAR* Context = TEXT("Context");
		FRound* Match = DT_Matches->FindRow<FRound>(Names[MatchCount], Context);
		MatchCount++;

		if (Player)
		{
			auto PlayerHealth = Player->GetComponentByClass<UVRHealthComponent>();
			if (ensure(PlayerHealth))
			{
				PlayerHealth->SetMaxHealth(Match->Health);
			}
		}
		if (Enemy)
		{
			auto EnemyHealth = Enemy->GetComponentByClass<UVRHealthComponent>();

			if (ensure(EnemyHealth))
			{
				EnemyHealth->SetMaxHealth(Match->Health);
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
		{
			HealthComp->InflictDamage();
			if (HealthComp->IsDead())
			{
				HealthComp->OnDead.Broadcast();
				bMatchOver = true;
			}
			else {
				HealthComp->OnHealthChanged.Broadcast(true, HealthComp->GetMaxHealth());
			}
		}

		ACharacter* CharacterHit = Cast<ACharacter>(ActorAimed);
		ACharacter* EnemyHit = Cast<ACEnemy>(ActorAimed);
		if (CharacterHit)
		{
			ChangeLifeLightColor(CharacterHit, FLinearColor::Red);
		}
		else if(EnemyHit)
		{
			ChangeLifeLightColor(EnemyHit, FLinearColor::Red);
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
	}
}

bool ADVRGameModeBase::IsMatchOver() const
{
	return bMatchOver;
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
