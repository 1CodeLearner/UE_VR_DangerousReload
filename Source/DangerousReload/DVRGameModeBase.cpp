// Fill out your copyright notice in the Description page of Project Settings.


#include "DVRGameModeBase.h"
#include "Justin/VRCharacter.h"
#include "Justin/VRHealthComponent.h"
#include "JINA/CEnemy.h"
#include "EngineUtils.h"
#include "JINA/CSpotLightActor.h"
#include "Justin/VRInteractables/VRInteractableActor_Pistol.h"
#include "VRGameStateBase.h"
#include <Components/SpotLightComponent.h>
#include "Justin/WidgetDisplay_AmmoCount.h"
#include "Justin/WorldUI/WidgetDisplayActor_ButtonPress.h"
#include "Justin/WorldUI/WidgetDisplayActor_Turns.h"

ADVRGameModeBase::ADVRGameModeBase()
{
	bIsInitialized = false;
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

	for (TActorIterator<AWidgetDisplay_AmmoCount> Iter(GetWorld()); Iter; ++Iter)
	{
		AmmoCountWidgetActor = *Iter;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Success %s"), *GetNameSafe(AmmoCountWidgetActor)), false);
	}

	for (TActorIterator<AWidgetDisplayActor_ButtonPress> Iter(GetWorld()); Iter; ++Iter)
	{
		ButtonPressWidgetActor = *Iter;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Success %s"), *GetNameSafe(ButtonPressWidgetActor)), false);
	}

	for (TActorIterator<AWidgetDisplayActor_Turns> Iter(GetWorld()); Iter; ++Iter)
	{
		TurnsWidgetActor = *Iter;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Success %s"), *GetNameSafe(TurnsWidgetActor)), false);
	}
}

void ADVRGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(Pistol))
	{
		Pistol->OnWeaponPickedUp.AddUObject(this, &ADVRGameModeBase::OnPickedUp);
		Pistol->OnWeaponDropped.AddUObject(this, &ADVRGameModeBase::OnDropped);
		Pistol->OnFired.BindUObject(this, &ADVRGameModeBase::OnFired);
		PistolRespawnTransform = Pistol->GetActorTransform();
	}

	TArray<ACSpotLightActor*> lifeLight;
	lifeLight.Empty();
	for (TActorIterator<ACSpotLightActor> it(GetWorld()); it; ++it) {
		lifeLight.Add(*it);
	}
	if (!lifeLight.IsEmpty())
	{
		playerLifeSpotlight.Empty();
		for (int32 i = 0; i < 4; ++i)
		{
			playerLifeSpotlight.Add(Cast<ACSpotLightActor>(lifeLight[i]));
		}
		for (int32 i = 0; i < 4; ++i)
		{

			enemyLifeSpotlight.Add(Cast<ACSpotLightActor>(lifeLight[i + 4]));
		}
	}

	auto Temp = GetGameState<AVRGameStateBase>();
	if (ensure(Temp))
	{
		VRGameState = Temp;
		VRGameState->OnMatchStateChanged.AddUObject(this, &ADVRGameModeBase::OnMatchStateChanged);
		VRGameState->ChangeMatchStateTo(EMatchState::EMATCH_Menu);
	}
}


void ADVRGameModeBase::OnMatchStateChanged(EMatchState CurrentMatchState)
{
	switch (CurrentMatchState)
	{
	case EMatchState::EMATCH_Menu:
	{
		/*
		*1. Turn off player and enemy Life Lights
		*/
		DisplayButtonPress(true);
		break;
	}
	case EMatchState::EMATCH_Start:
	{
		if (!bIsInitialized)
		{
			InitializeParticipants();
			bIsInitialized = true;
		}
		Pistol->Reload();
		VRGameState->SetCurrentTurn(Player);
		DisplayAmmoCount(true);
		SwitchStateOnTimer(EMatchState::EMATCH_OnGoing, 2.f);
		break;
	}
	case EMatchState::EMATCH_OnGoing:
	{
		if (VRGameState->CameFromStartState())
		{
			DisplayTurnAndPickup(true);
		}
		break;
	}
	case EMatchState::EMATCH_SwitchTurn:
	{
		if (VRGameState->IsCurrentTurn(Player) && Pistol->IsHeld())
			DisplaySettingDownWeapon(true);
		break;
	}
	case EMatchState::EMATCH_RoundReset:
	{
		if (VRGameState->IsCurrentTurn(Player) && Pistol->IsHeld())
			DisplaySettingDownWeapon(true);
		break;
	}
	case EMatchState::EMATCH_StageClear:
	{
		/*Stage clear sound effect*/
		VRGameState->StageCount++;
		SwitchStateOnTimer(EMatchState::EMATCH_RoundReset, 5.f);
		break;
	}
	case EMatchState::EMATCH_StageLost:
	{
		/*Play a Sequence or losing sound effect*/
		SwitchStateOnTimer(EMatchState::EMATCH_RoundReset, 5.f);
		break;
	}
	case EMatchState::EMATCH_GameClear:
	{
		//Play slow motion for 2 seconds
		//after slow motion, turn off light
		//Display game clear message
		SwitchStateOnTimer(EMatchState::EMATCH_Menu, 5.f);
		break;
	}
	case EMatchState::EMATCH_GameOver:
	{
		//turn off light
		//Display game over message
		SwitchStateOnTimer(EMatchState::EMATCH_Menu, 5.f);
		break;
	}
	case EMatchState::EMATCH_Default:
	{
		break;
	}
	}
}

void ADVRGameModeBase::DisplayButtonPress(bool bEnable)
{
	ButtonPressWidgetActor->DisplayUI(bEnable);
}

void ADVRGameModeBase::DisplayAmmoCount(bool bEnable)
{
	//Display Ammo count.
	//With UI or on the table in game
	AmmoCountWidgetActor->DisplayUI(bEnable);
}

void ADVRGameModeBase::DisplayTurnAndPickup(bool bEnable)
{
	TurnsWidgetActor->DisplayUI(bEnable);
}

void ADVRGameModeBase::DisplaySettingDownWeapon(bool bEnable)
{
	//Display an UI that tells player to set their weapon down on table
}

void ADVRGameModeBase::DisplayRackingWeapon(bool bEnable)
{
}

void ADVRGameModeBase::SwitchStateOnTimer(EMatchState MatchState, float InRate)
{
	FTimerHandle Handle;
	FTimerDelegate SwitchStateDelegate;
	SwitchStateDelegate.BindUObject(this, &ADVRGameModeBase::PreSwitchingState, MatchState);
	GetWorld()->GetTimerManager().SetTimer(Handle, SwitchStateDelegate, InRate, false);
}

void ADVRGameModeBase::PreSwitchingState(EMatchState MatchState)
{
	switch (MatchState)
	{
	case EMatchState::EMATCH_Menu:
	{
		break;
	}
	case EMatchState::EMATCH_Start:
	{
		DisplayButtonPress(false);
		break;
	}
	case EMatchState::EMATCH_OnGoing:
	{
		DisplayAmmoCount(false);
		break;
	}
	case EMatchState::EMATCH_SwitchTurn:
	{
		break;
	}
	case EMatchState::EMATCH_RoundReset:
	{
		break;
	}
	case EMatchState::EMATCH_StageClear:
	{
		break;
	}
	case EMatchState::EMATCH_StageLost:
	{
		break;
	}
	case EMatchState::EMATCH_GameClear:
	{
		break;
	}
	case EMatchState::EMATCH_GameOver:
	{
		break;
	}
	case EMatchState::EMATCH_Default:
	{
		break;
	}
	}

	VRGameState->ChangeMatchStateTo(MatchState);
}

void ADVRGameModeBase::InitializeParticipants()
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
	}
}

void ADVRGameModeBase::OnPickedUp()
{
	DisplayTurnAndPickup(false);
}

void ADVRGameModeBase::OnDropped()
{
	/*
	 *	Do line trace below pistol
	 *	if line trace does not hit table
			Call Pistol->SetActorTransform(PistolRespawnTransform); //

	 */

	Pistol->SetActorTransform(PistolRespawnTransform);

	if (VRGameState->IsMatchState(EMatchState::EMATCH_RoundReset))
	{
		SwitchStateOnTimer(EMatchState::EMATCH_Start, 1.5f);
	}

	if (VRGameState->IsMatchState(EMatchState::EMATCH_SwitchTurn))
	{
		SwitchStateOnTimer(EMatchState::EMATCH_OnGoing, 1.5f);
	}
}

void ADVRGameModeBase::OnFired(AActor* ActorInstigator, AActor* ActorAimed, bool bIsLiveRound)
{
	UVRHealthComponent* HealthComp = ActorAimed->GetComponentByClass<UVRHealthComponent>();
	bool bPlayerHit = false;

	//Handle Health
	if (bIsLiveRound)
	{
		if (ensure(HealthComp))
		{
			HealthComp->InflictDamage();
			HealthComp->OnHealthChanged.Broadcast(true, HealthComp->GetMaxHealth());

			AVRCharacter* CharacterHit = Cast<AVRCharacter>(ActorAimed);
			ACEnemy* EnemyHit = Cast<ACEnemy>(ActorAimed);

			if (CharacterHit)
			{
				ChangeLifeLightColor(CharacterHit, FLinearColor::Red);
				bPlayerHit = true;
			}
			else if (EnemyHit)
			{
				ChangeLifeLightColor(EnemyHit, FLinearColor::Red);
			}
		}
	}

	if (HealthComp->IsDead())
	{
		if (bPlayerHit)
		{
			PreSwitchingState(EMatchState::EMATCH_GameOver);
			/*if (IsFinalStage())
			{
				PreSwitchingState(EMatchState::EMATCH_GameOver);
			}
			else
			{
				PreSwitchingState(EMatchState::EMATCH_StageLost);
			}*/
		}
		else
		{
			PreSwitchingState(EMatchState::EMATCH_GameClear);
			/*if (IsFinalStage())
			{
				PreSwitchingState(EMatchState::EMATCH_GameClear);
			}
			else
			{
				PreSwitchingState(EMatchState::EMATCH_StageClear);
			}*/
		}
	}
	else if (Pistol->IsEmpty())
	{
		PreSwitchingState(EMatchState::EMATCH_RoundReset);
	}
	else
	{
		//Handle turns
		if (bIsLiveRound)
		{
			//Switch Turns regardless of who was shot
			if (ActorInstigator == Player)
			{
				VRGameState->SetCurrentTurn(Enemy);
			}
			else
			{
				VRGameState->SetCurrentTurn(Player);
			}

			PreSwitchingState(EMatchState::EMATCH_SwitchTurn);
		}
		else
		{
			//Only switch turn if a blank was shot at other participant
			if (!VRGameState->IsCurrentTurn(ActorAimed))
			{
				VRGameState->SetCurrentTurn(ActorAimed);

				PreSwitchingState(EMatchState::EMATCH_SwitchTurn);
			}
		}
	}
}

bool ADVRGameModeBase::IsFinalStage() const
{
	return VRGameState->GetMatchCount() >= DT_Matches->GetRowNames().Num() - 1;
}

void ADVRGameModeBase::ChangeLifeLightColor(ACharacter* target, FLinearColor color)
{
	if (Cast<AVRCharacter>(target) != nullptr) {
		for (int32 i = 0; i < playerLifeSpotlight.Num(); i++) {
			if (playerLifeSpotlight[i] != nullptr && playerLifeSpotlight[i]->spotLight->GetLightColor() == color) continue;
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
			if (enemyLifeSpotlight[i]->spotLight->GetLightColor() == color) continue;
			else
			{
				enemyLifeSpotlight[i]->spotLight->SetLightColor(color);
				break;
			}
		}
	}
}
