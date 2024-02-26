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
public:
	AVRInteractableActor_Pistol();
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnPickup(AActor* InstigatorA) override;
	virtual void OnRelease(AActor* InstigatorA) override;
	virtual void OnInteract(AActor* InstigatorA) override;

	void RackPistol();
	bool IsRoundsEmpty() const;
	void Reload();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Pistol")
	TObjectPtr<UAnimSequence> FireSequenceAnim;

	void OnMatchChanged();

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<USoundBase> RackingSound;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<USoundBase> EmptyGunSound;

private:
	bool bCanFire;
	void FindActorInLOS();
	UPROPERTY()
	AActor* ActorInLOS;

	int LiveRounds;
	TArray<bool> Rounds;
	int RoundCounter; 

	UPROPERTY()
	TObjectPtr<ADVRGameModeBase> GameMode;
};
