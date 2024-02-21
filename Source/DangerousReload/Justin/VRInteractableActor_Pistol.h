// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRInteractableActor.h"
#include "VRInteractableActor_Pistol.generated.h"

/**
 *
 */
UCLASS()
class DANGEROUSRELOAD_API AVRInteractableActor_Pistol : public AVRInteractableActor
{
	GENERATED_BODY()

	virtual void OnPickup(AActor* InstigatorA) override;
	virtual void OnRelease(AActor* InstigatorA) override;
	virtual void OnInteract(AActor* InstigatorA) override;
protected:
	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Pistol")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Pistol")
	TObjectPtr<UAnimSequence> FireSequenceAnim;
};
