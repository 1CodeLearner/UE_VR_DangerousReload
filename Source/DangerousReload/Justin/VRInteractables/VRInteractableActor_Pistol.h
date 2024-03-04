// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRInteractableActor.h"
#include "VRInteractableActor_Pistol.generated.h"

/**
 *
 */

DECLARE_MULTICAST_DELEGATE(FWeaponDroppedDelegate);
DECLARE_MULTICAST_DELEGATE(FWeaponPickedUpDelegate);
DECLARE_DELEGATE_ThreeParams(FFiredDelegate, AActor* ActorInstigator, AActor* ActorAimed, bool bIsLiveRound);

UCLASS()
class DANGEROUSRELOAD_API AVRInteractableActor_Pistol : public AVRInteractableActor
{
	GENERATED_BODY()
public:
	AVRInteractableActor_Pistol();

	FWeaponDroppedDelegate OnWeaponDropped;
	FWeaponPickedUpDelegate OnWeaponPickedUp;
	FFiredDelegate OnFired;


	virtual void Tick(float DeltaTime) override;

	virtual void OnPickup(AActor* InstigatorA) override;
	virtual void OnRelease(AActor* InstigatorA) override;
	virtual void OnInteract(AActor* InstigatorA) override;

	bool IsRacked() const;
	void RackPistol();
	bool IsEmpty() const;
	bool IsHeld() const;

	void SetActorInLOS(AActor* OtherActor);

	// have to use in BP Desk, so changed - by JINA
	UFUNCTION(BlueprintCallable)
	float GetLiveRounds() const;
	UFUNCTION(BlueprintCallable)
	float GetTotalRounds() const;
	UFUNCTION(BlueprintCallable)
	float GetRemainingRounds() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<UAnimSequence> FireSequenceAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<USoundBase> RackingSound;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<USoundBase> EmptyGunSound;

	virtual void OnMatchChanged(EMatchState CurrentMatchState) override;

private:
	bool IsActive() const;
	//Changed by match states (in AVRGameStateBase)
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
	int RoundsIndex;

	//Only this class can reload the weapon
	friend class ADVRGameModeBase;

	void Reload();
};
