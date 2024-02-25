// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSpotLightActor.generated.h"

UCLASS()
class DANGEROUSRELOAD_API ACSpotLightActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACSpotLightActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
