// Fill out your copyright notice in the Description page of Project Settings.


#include "VRInteractableActor_Pistol.h"

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
		SKMComp->PlayAnimation(FireSequenceAnim, false);
		/*auto AnimInst = SKMComp->GetAnimInstance();
		if (ensure(AnimInst)) 
		{
			AnimInst->Montage_Play(FireMontage);
		}*/
	}
}
