// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Justin/VRInteractables/VRInteractableActor.h"
#include "CHealthItem.generated.h"

/**
 * 
 */
UCLASS()
class DANGEROUSRELOAD_API ACHealthItem : public AVRInteractableActor
{
	GENERATED_BODY()
	
	ACHealthItem();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPickup(AActor* InstigatorA) override;
	virtual void OnInteract(AActor* InstigatorA) override;
	virtual void OnRelease(AActor* InstigatorA) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* btnComp;
	class AVRCharacter* player;
	class ACEnemy* enemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystem> btnEffect;
};
