// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Justin/VRInteractableActor.h"
#include "CGun.generated.h"

/**
 * 
 */
UCLASS()
class DANGEROUSRELOAD_API ACGun : public AVRInteractableActor
{
	GENERATED_BODY()
	
public:
	UParticleSystem* explosion;

public:
	void Shoot(ACharacter* target);
};
