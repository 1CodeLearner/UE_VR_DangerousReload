// Fill out your copyright notice in the Description page of Project Settings.


#include "VRHandAnim.h"
#include "VRCharacter.h"

void UVRHandAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	bMirrored = false;
	VRPlayer = Cast<AVRCharacter>(GetOwningActor());
}

void UVRHandAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (VRPlayer) {
		bMirrored ? RightGripAlpha = VRPlayer->LeftHandGripValue : RightGripAlpha = VRPlayer->RightHandGripValue;
		
		TargetIndexTouchValue = bMirrored ? VRPlayer->LHTriggerTouch : VRPlayer->RHTriggerTouch;
		CurrentIndexTouchValue = FMath::Lerp(CurrentIndexTouchValue, TargetIndexTouchValue, DeltaSeconds);

		bMirrored ? IndexTriggerAlpha = VRPlayer->LHTriggerAxis : IndexTriggerAlpha = VRPlayer->RHTriggerAxis;
	}


}
