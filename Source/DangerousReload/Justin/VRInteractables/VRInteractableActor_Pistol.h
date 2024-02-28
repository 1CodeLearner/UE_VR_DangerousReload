// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRInteractableActor.h"
#include "VRInteractableActor_Pistol.generated.h"

/**
 *
 */

 DECLARE_DELEGATE(FWeaponDroppedDelegate);

UCLASS()
class DANGEROUSRELOAD_API AVRInteractableActor_Pistol : public AVRInteractableActor
{
	GENERATED_BODY()
public:
	AVRInteractableActor_Pistol();

	FWeaponDroppedDelegate OnWeaponDropped;

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

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<USoundBase> RackingSound;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<USoundBase> EmptyGunSound;

	virtual void OnMatchChanged(EMatchState CurrentMatchState) override;

private:
	bool IsActive() const;
	//Changed by game & match states
	bool bIsActive;
	//Changed by handling the weapon
	bool bCanFire;
	//Changed by Racking the weapon
	bool bRacked;
	//Check if player or enemy is currently holding the weapon
	bool bIsHeld;

	UPROPERTY()
	AActor* ActorInLOS;
	void FindActorInLOS();

	int LiveRounds;
	TArray<bool> Rounds;
	int RoundCounter; 

	FTimerHandle RespawnHandle;
	UFUNCTION()
	void RespawnWeapon();
};
