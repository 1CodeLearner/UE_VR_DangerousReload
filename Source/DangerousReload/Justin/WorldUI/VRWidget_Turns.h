// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../VRWidget.h"
#include "VRWidget_Turns.generated.h"

class UTextBlock; 
/**
 *
 */
UCLASS()
class DANGEROUSRELOAD_API UVRWidget_Turns : public UVRWidget
{
	GENERATED_BODY()
public:
	virtual void Display(bool bEnable) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "Settings")
	TObjectPtr<UTextBlock> TurnsText;


};
