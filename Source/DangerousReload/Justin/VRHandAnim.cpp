// Fill out your copyright notice in the Description page of Project Settings.


#include "VRHandAnim.h"
#include "VRCharacter.h"

UVRHandAnim::UVRHandAnim()
{
	LerpRate = 1.0f;
}

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
		
		bMirrored ? IndexTriggerAlpha = VRPlayer->LHTriggerAxis : IndexTriggerAlpha = VRPlayer->RHTriggerAxis;

		TargetIndexTouchValue = bMirrored ? VRPlayer->LHTriggerTouch : VRPlayer->RHTriggerTouch;
		CurrentIndexTouchValue = FMath::Lerp(CurrentIndexTouchValue, TargetIndexTouchValue, DeltaSeconds * LerpRate);

		TargetThumbTouchValue = bMirrored ? VRPlayer->LHThumbTouch : VRPlayer->RHThumbTouch;
		CurrentThumbTouchValue = FMath::Lerp(CurrentThumbTouchValue, TargetThumbTouchValue, DeltaSeconds * LerpRate);

		bGrippingWeapon = bMirrored ? false : VRPlayer->bIsHoldingWeapon;
	}


}
