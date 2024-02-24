// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SpotLight.h"
#include "CSpotLight.generated.h"

/**
 * 
 */
UCLASS()
class DANGEROUSRELOAD_API ACSpotLight : public ASpotLight
{
	GENERATED_BODY()
public:
	ACSpotLight();

public:
	UPROPERTY(VisibleAnywhere, Category = "Life Light")
	class USpotLightComponent* spotLight;

	UPROPERTY(EditAnywhere, Category = "Life Light")
	float lightIntensity;

	UPROPERTY(EditAnywhere, Category = "Life Light")
	float radiusSize;

	UPROPERTY(EditAnywhere, Category = "Life Light")
	float cornAngle;

public:
	void ChangeColor(FLinearColor color);
};
