// Fill out your copyright notice in the Description page of Project Settings.


#include "VRInteractableActor_Pistol.h"

#include "DangerousReload/DVRgameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../../DVRgameModeBase.h"
#include "../BFL_Logging.h"

//static TAutoConsoleVariable<bool> CVarMaxLiveRounds(TEXT("jk.MaxLiveRounds"), false, TEXT("Set all rounds to live rounds")

AVRInteractableActor_Pistol::AVRInteractableActor_Pistol()
{
	RoundCounter = 0;
	bIsActive = false;
	bCanFire = true;
	bIsItem = false;
	bRacked = true;
	bIsHeld = false;
}

void AVRInteractableActor_Pistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsActive() && GetOwner())
	{
		FindActorInLOS();
	}
	for (int i = 0; i < Rounds.Num(); ++i)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Magenta, FString::Printf(TEXT("Round%d: %s"), i, Rounds[i] ? TEXT("True") : TEXT("False")));
	}
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Magenta, FString::Printf(TEXT("CurrentRound: %d"), RoundCounter));
}

void AVRInteractableActor_Pistol::OnMatchChanged(EMatchState CurrentMatchState)
{
	Super::OnMatchChanged(CurrentMatchState);

	if (CurrentMatchState == EMatchState::EMATCH_SwitchTurn)
	{
		bIsActive = false;
	}
	if (CurrentMatchState == EMatchState::EMATCH_OnGoing)
	{
		bIsActive = true;
	}
	if (CurrentMatchState == EMatchState::EMATCH_Start)
	{
		bIsActive = true;
		Reload();
	}
}

void AVRInteractableActor_Pistol::OnPickup(AActor* InstigatorA)
{
	if (VRGameState->CurrentTurn == InstigatorA)
	{
		Super::OnPickup(InstigatorA);
		if (RespawnHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(RespawnHandle);
		}
		bIsHeld = true;
	}
}

void AVRInteractableActor_Pistol::OnRelease(AActor* InstigatorA)
{
	if (bIsHeld) 
	{
		Super::OnRelease(InstigatorA);
		
		UBFL_Logging::GEngineLog(FString::Printf(TEXT("Putting weapon back on the table...")));
		if (RespawnHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(RespawnHandle);
		}
		GetWorld()->GetTimerManager().SetTimer(RespawnHandle, this, &AVRInteractableActor_Pistol::RespawnWeapon, 2.f, false);

		bIsHeld = false;
	}
}

void AVRInteractableActor_Pistol::OnInteract(AActor* InstigatorA)
{
	Super::OnInteract(InstigatorA);
	if (IsActive() && ActorInLOS)
	{
		if (RoundCounter < Rounds.Num())
		{
			if (Rounds[RoundCounter])
			{
				SKMComp->PlayAnimation(FireSequenceAnim, false);
			}
			else
			{
				UGameplayStatics::PlaySoundAtLocation(GetOwner(), EmptyGunSound, GetOwner()->GetActorLocation(), FRotator::ZeroRotator);
			}
			gameMode->OnFired(GetOwner(), ActorInLOS, Rounds[RoundCounter]);
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(GetOwner(), EmptyGunSound, GetOwner()->GetActorLocation(), FRotator::ZeroRotator);
		}
		bCanFire = false;
		bRacked = false;
	}
}

void AVRInteractableActor_Pistol::RackPistol()
{
	if (!bRacked) {
		bCanFire = true;
		bRacked = true;
		RoundCounter++;

		if (ensure(RackingSound))
		{
			UGameplayStatics::PlaySoundAtLocation(GetOwner(), RackingSound, GetOwner()->GetActorLocation(), FRotator::ZeroRotator);
		}
	}
}

bool AVRInteractableActor_Pistol::IsRoundsEmpty() const
{
	return RoundCounter >= Rounds.Num();
}

void AVRInteractableActor_Pistol::Reload()
{
	int32 totalRounds = FMath::RandRange(2, 8);
	int32 liveRounds = totalRounds / 2;

	int32 operations = FMath::RandRange(0, 2);
	if (totalRounds > 4)
	{
		switch (operations)
		{
		case 0:
		{
			break;
		}
		case 1:
		{
			liveRounds++;
			break;
		}
		case 2:
		{
			liveRounds--;
			break;
		}
		}
	}

	LiveRounds = liveRounds;

	Rounds.Reset();
	Rounds.SetNum(totalRounds);
	int Counter = 0;
	do
	{
		int RandVal = FMath::RandRange(0, Rounds.Num() - 1);
		if (Rounds[RandVal] != true)
		{
			Rounds[RandVal] = true;
			Counter++;
		}
	} while (Counter != LiveRounds);

	gameMode->bulletCount = totalRounds;
	bCanFire = true;
	RoundCounter = 0;
}

bool AVRInteractableActor_Pistol::IsActive() const
{
	return bIsActive && bCanFire;
}

void AVRInteractableActor_Pistol::FindActorInLOS()
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorRightVector() * 20000.f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	//Check if Pistol is aiming at participant
	FCollisionObjectQueryParams GameTraceParams;
	GameTraceParams.AddObjectTypesToQuery(ECC_GameTraceChannel3);
	FHitResult PartiHit;
	FCollisionShape	Shape;
	Shape.MakeSphere(24.f);

	bool bParticipantHit = GetWorld()->LineTraceSingleByObjectType(PartiHit, Start, End, ECC_GameTraceChannel3, Params);

	if (bParticipantHit)
	{
		//Check if any objects are blocking Pistol's line of fire
		FCollisionObjectQueryParams VisibilityParams;
		VisibilityParams.AddObjectTypesToQuery(ECC_Visibility);
		FHitResult VisiHit;

		bool bVisibilityHit = GetWorld()->LineTraceSingleByChannel(VisiHit, Start, PartiHit.ImpactPoint, ECC_Visibility, Params);

		if (!bVisibilityHit)
		{
			ActorInLOS = PartiHit.GetActor();
			DrawDebugSphere(GetWorld(), PartiHit.ImpactPoint, 6.f, 24, FColor::Red, false, 1.f);
			return;
		}
		else {
			DrawDebugSphere(GetWorld(), VisiHit.ImpactPoint, 6.f, 24, FColor::Blue, false, 1.f);
		}
	}

	ActorInLOS = nullptr;
	DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 2.f);

}

void AVRInteractableActor_Pistol::RespawnWeapon()
{
	OnWeaponDropped.Execute();
}
