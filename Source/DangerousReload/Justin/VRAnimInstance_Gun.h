// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VRAnimInstance_Gun.generated.h"


class AVRCharacter; 
/**
 *
 */
UCLASS()
class DANGEROUSRELOAD_API UVRAnimInstance_Gun : public UAnimInstance
{
	GENERATED_BODY()
protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


public:
	bool bIsFiring;
	TObjectPtr <AVRCharacter> Player;
};
