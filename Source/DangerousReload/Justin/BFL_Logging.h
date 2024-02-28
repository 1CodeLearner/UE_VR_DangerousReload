// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_Logging.generated.h"

/**
 *
 */

UENUM()
enum class EHand : uint8
{
	RIGHT UMETA(DisplayName="Right"),
	LEFT UMETA(DisplayName = "Left")
};

UCLASS()
class DANGEROUSRELOAD_API UBFL_Logging : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static void Log(AActor* Player, FString Message, EHand HandEnum = EHand::LEFT);
	static void GEngineLog(FString Message, FColor Color = FColor::Magenta);
};
