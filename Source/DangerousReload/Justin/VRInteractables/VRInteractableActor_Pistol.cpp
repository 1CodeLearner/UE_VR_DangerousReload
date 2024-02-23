// Fill out your copyright notice in the Description page of Project Settings.


#include "VRInteractableActor_Pistol.h"

AVRInteractableActor_Pistol::AVRInteractableActor_Pistol()
{
	bCanFire = false;
}

void AVRInteractableActor_Pistol::BeginPlay()
{
	Super::BeginPlay();
}

void AVRInteractableActor_Pistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetOwner()) 
	{
		CheckCanFire();
	}
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
	if (SKMComp)
	{
		if (bCanFire)
		{
			SKMComp->PlayAnimation(FireSequenceAnim, false);
		}
	}
}

void AVRInteractableActor_Pistol::CheckCanFire()
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
			bCanFire = true;
			DrawDebugSphere(GetWorld(), PartiHit.ImpactPoint, 6.f, 24, FColor::Red, false, 1.f);
			return;	
		}
		else {
			DrawDebugSphere(GetWorld(), VisiHit.ImpactPoint, 6.f, 24, FColor::Blue, false, 1.f);
		}
	}

	bCanFire = false;
	DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 2.f);

}
