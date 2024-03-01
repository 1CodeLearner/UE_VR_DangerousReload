// Fill out your copyright notice in the Description page of Project Settings.


#include "DVRGameModeBase.h"
#include "Justin/VRCharacter.h"
#include "Justin/VRHealthComponent.h"
#include "JINA/CEnemy.h"
#include "EngineUtils.h"
#include "JINA/CSpotLightActor.h"
#include "Justin/VRInteractables/VRInteractableActor_Pistol.h"
#include "VRGameStateBase.h"

ADVRGameModeBase::ADVRGameModeBase()
{
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

	for (TActorIterator<AVRInteractableActor_Pistol> Iter(GetWorld()); Iter; ++Iter)
	{
		if (!Pistol)
		{
			Pistol = *Iter;
		}
	}
}

void ADVRGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	auto Temp = GetGameState<AVRGameStateBase>();
	if (ensure(Temp))
	{
		VRGameState = Temp;
		VRGameState->OnMatchStateChanged.AddUObject(this, &ADVRGameModeBase::OnMatchStateChanged);
		VRGameState->ChangeMatchStateTo(EMatchState::EMATCH_Menu);
	}

	if (ensure(Pistol))
	{
		Pistol->OnWeaponDropped.BindUObject(this, &ADVRGameModeBase::RespawnPistol);
		PistolRespawnTransform = Pistol->GetActorTransform();
	}

	TArray<ACSpotLightActor*> lifeLight;
	for (TActorIterator<ACSpotLightActor> it(GetWorld()); it; ++it) {
		lifeLight.Add(*it);
	}
	if (!lifeLight.IsEmpty())
	{
		for (int32 i = 0; i < 4; ++i)
		{
			playerLifeSpotlight.Add(Cast<ACSpotLightActor>(lifeLight[i]));
		}
		for (int32 i = 0; i < 4; ++i)
		{

			enemyLifeSpotlight.Add(Cast<ACSpotLightActor>(lifeLight[i + 4]));
		}
	}
}


void ADVRGameModeBase::OnMatchStateChanged(EMatchState CurrentMatchState)
{
	if (CurrentMatchState == EMatchState::EMATCH_Start)
	{
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ADVRGameModeBase::StartMatch, 2.f, false);
	}
}

void ADVRGameModeBase::StartMatch()
{
	if (ensure(DT_Matches) && ensure(VRGameState))
	{
		TArray<FName> Names = DT_Matches->GetRowNames();
		const TCHAR* Context = TEXT("Context");
		FRound* Match = DT_Matches->FindRow<FRound>(Names[VRGameState->GetMatchCount()], Context);

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
		VRGameState->SetCurrentTurn(Player);
	}
}


void ADVRGameModeBase::OnFired(AActor* ActorInstigator, AActor* ActorAimed, bool bIsLiveRound)
{
	UVRHealthComponent* HealthComp = ActorAimed->GetComponentByClass<UVRHealthComponent>();
	//Handle Health
	if (bIsLiveRound)
	{
		if (ensure(HealthComp))
		{
			HealthComp->InflictDamage();
			if (HealthComp->IsDead())
			{
				HealthComp->OnDead.Broadcast();
			}
			else
			{
				HealthComp->OnHealthChanged.Broadcast(true, HealthComp->GetMaxHealth());

				ACharacter* CharacterHit = Cast<ACharacter>(ActorAimed);
				ACharacter* EnemyHit = Cast<ACEnemy>(ActorAimed);
				if (CharacterHit)
				{
					ChangeLifeLightColor(CharacterHit, FLinearColor::Red);
				}
				else if (EnemyHit)
				{
					ChangeLifeLightColor(EnemyHit, FLinearColor::Red);
				}
			}
		}
	}


	if (HealthComp->IsDead())
	{
		VRGameState->ChangeMatchStateTo(EMatchState::EMATCH_Stop);
	}
	else if (!Pistol->CanFire())
	{
		VRGameState->ChangeMatchStateTo(EMatchState::EMATCH_RoundReset);
	}
	else
	{
		//Handle turns
		if (bIsLiveRound)
		{
			//Switch Turns regardless who was shot
			if (ActorInstigator == Player)
			{
				VRGameState->SetCurrentTurn(Enemy);
			}
			else
			{
				VRGameState->SetCurrentTurn(Player);
			}
			VRGameState->ChangeMatchStateTo(EMatchState::EMATCH_SwitchTurn);

		}
		else
		{
			//Switch turn only if a blank was shot at other participant
			if (VRGameState->GetCurrentTurn() != ActorAimed)
			{
				VRGameState->SetCurrentTurn(ActorAimed);
				VRGameState->ChangeMatchStateTo(EMatchState::EMATCH_SwitchTurn);
			}
		}
	}
}


void ADVRGameModeBase::ChangeLifeLightColor(ACharacter* target, FLinearColor color)
{
	if (Cast<AVRCharacter>(target) != nullptr) {
		for (int32 i = 0; i < playerLifeSpotlight.Num(); i++) {
			//if (playerLifeSpotlight[i]->spotLight->GetLightColor() == color) continue;
			//else
			//{
			//	playerLifeSpotlight[i]->spotLight->SetLightColor(color);
			//	break;
			//}
		}
	}
	else
	{
		for (int32 i = 0; i < enemyLifeSpotlight.Num(); i++) {
			/*if(enemyLifeSpotlight[i]->spotLight->GetLightColor() == color) continue;
			else
			{
				enemyLifeSpotlight[i]->spotLight->SetLightColor(color);
				break;
			}*/
		}
	}
}

void ADVRGameModeBase::RespawnPistol()
{
	//Move Pistol to original Location
	Pistol->SetActorTransform(PistolRespawnTransform);

	if (VRGameState->IsMatchState(EMatchState::EMATCH_SwitchTurn))
	{
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ADVRGameModeBase::SwitchTurns, 2.f, false);
	}
	else if (VRGameState->IsMatchState(EMatchState::EMATCH_RoundReset))
	{
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ADVRGameModeBase::RestartMatch, 2.f, false);
	}
}

void ADVRGameModeBase::SwitchTurns()
{
	VRGameState->ChangeMatchStateTo(EMatchState::EMATCH_OnGoing);
}

void ADVRGameModeBase::RestartMatch()
{
	VRGameState->ChangeMatchStateTo(EMatchState::EMATCH_Start);
}
