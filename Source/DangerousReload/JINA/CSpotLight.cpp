// Fill out your copyright notice in the Description page of Project Settings.


#include "CSpotLight.h"
#include <Components/SpotLightComponent.h>

ACSpotLight::ACSpotLight()
{
	lightIntensity = 300;
	radiusSize = 300;
	cornAngle = 10;

	spotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spot Light"));
	spotLight->Intensity = lightIntensity;
	spotLight->SetActive(true);
	RootComponent = spotLight;
	spotLight->SetLightColor(FLinearColor::White);
	spotLight->SetAttenuationRadius(radiusSize);
	spotLight->SetOuterConeAngle(cornAngle);
	spotLight->SetInnerConeAngle(cornAngle);
}

void ACSpotLight::ChangeColor(FLinearColor color)
{
	spotLight->SetLightColor(color);
}
