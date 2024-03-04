// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VRHandAnim.generated.h"

class AVRCharacter;

/**
 *
 */
UCLASS()
class DANGEROUSRELOAD_API UVRHandAnim : public UAnimInstance
{
	GENERATED_BODY()
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
protected:
	friend class AVRCharacter;

	UPROPERTY()
	TObjectPtr<AVRCharacter> VRPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float RightGripAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float IndexTriggerAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float CurrentIndexTouchValue;
	UPROPERTY()
	float TargetIndexTouchValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	bool bMirrored;
};
