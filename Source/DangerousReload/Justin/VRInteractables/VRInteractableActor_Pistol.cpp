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
		//Test What gets printed Out!!!
		SKMComp->PlayAnimation(FireSequenceAnim, false);
		UE_LOG(LogTemp, Warning, TEXT("Owner: %s"), *GetNameSafe(GetOwner()));
		UE_LOG(LogTemp, Warning, TEXT("GetAttachParent: %s"), *GetNameSafe(GetOwner()));
	}
}

bool AVRInteractableActor_Pistol::CanFire() const
{
	return false;
	//FHitResult Hit;
	//FVector Start = GetActorLocation();
	//FVector End = Start + GetActorForwardVector() * 10000.f;
	//FCollisionQueryParams Params;
	//Params.AddIgnoredActor()
	//FCollisionObjectQueryParams ObjectParams;

	//GetWorld()->LineTraceSingleByObjectType(Hit, GetActorLocation(), );

}
