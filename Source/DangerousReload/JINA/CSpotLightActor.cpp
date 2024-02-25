// Fill out your copyright notice in the Description page of Project Settings.


#include "CSpotLightActor.h"
#include <Components/SpotLightComponent.h>


// Sets default values
ACSpotLightActor::ACSpotLightActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

// Called when the game starts or when spawned
void ACSpotLightActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACSpotLightActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACSpotLightActor::ChangeColor(FLinearColor color)
{
	spotLight->SetLightColor(color);
}

