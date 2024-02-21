// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VRInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVRInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DANGEROUSRELOAD_API IVRInteractInterface
{
	GENERATED_BODY()
public:
	virtual void OnPickup(AActor* Instigator);
	virtual void OnRelease(AActor* Instigator);
	virtual void OnInteract(AActor* Instigator);
};
