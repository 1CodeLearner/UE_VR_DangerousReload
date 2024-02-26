// Fill out your copyright notice in the Description page of Project Settings.


#include "VRInteractableActor_Pistol.h"

#include "DangerousReload/DVRGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../../DVRGameModeBase.h"

//static TAutoConsoleVariable<bool> CVarMaxLiveRounds(TEXT("jk.MaxLiveRounds"), false, TEXT("Set all rounds to live rounds")

AVRInteractableActor_Pistol::AVRInteractableActor_Pistol()
{
	RoundCounter = 0;
	bCanFire = true;
	bIsItem = false;
}

void AVRInteractableActor_Pistol::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GameMode = GetWorld()->GetAuthGameMode<ADVRGameModeBase>();

	if (ensure(GameMode) && GameState)
	{
		GameState->OnMatchStateChanged.AddUObject(this, &AVRInteractableActor_Pistol::OnMatchChanged);
	}
}

void AVRInteractableActor_Pistol::BeginPlay()
{
	Super::BeginPlay();
}

void AVRInteractableActor_Pistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bCanFire && GetOwner())
	{
		FindActorInLOS();
	}
	for (int i = 0; i < Rounds.Num(); ++i)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Magenta, FString::Printf(TEXT("Round%d: %s"), i, Rounds[i] ? TEXT("True") : TEXT("False")));
	}
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Magenta, FString::Printf(TEXT("CurrentRound: %d"), RoundCounter));
}

void AVRInteractableActor_Pistol::OnPickup(AActor* InstigatorA)
{
	Super::OnPickup(InstigatorA);
}

void AVRInteractableActor_Pistol::OnRelease(AActor* InstigatorA)
{
	Super::OnRelease(InstigatorA);
}

void AVRInteractableActor_Pistol::OnInteract(AActor* InstigatorA)
{
	Super::OnInteract(InstigatorA);
	if (ActorInLOS && bCanFire)
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
			GameMode->OnFired(GetOwner(), ActorInLOS, Rounds[RoundCounter]);
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(GetOwner(), EmptyGunSound, GetOwner()->GetActorLocation(), FRotator::ZeroRotator);
		}
		bCanFire = false;
	}
}

void AVRInteractableActor_Pistol::RackPistol()
{
	bCanFire = true;
	RoundCounter++;

	if (ensure(RackingSound))
	{
		UGameplayStatics::PlaySoundAtLocation(GetOwner(), RackingSound, GetOwner()->GetActorLocation(), FRotator::ZeroRotator);
	}
}

bool AVRInteractableActor_Pistol::IsRoundsEmpty() const
{
	return RoundCounter >= Rounds.Num();
}

void AVRInteractableActor_Pistol::OnMatchChanged()
{
	if (GameState->MatchStateEnum == EMatchState::EMATCH_Start)
		Reload();
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

	GameMode->bulletCount = totalRounds;
	bCanFire = true;
	RoundCounter = 0;
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
