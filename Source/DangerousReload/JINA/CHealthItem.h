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

	virtual void Tick(float DeltaTime) override;
	virtual void OnInteract(AActor* InstigatorA) override;

public:
	class AVRPlayer* player;
	class ACEnemy* enemy;
};
