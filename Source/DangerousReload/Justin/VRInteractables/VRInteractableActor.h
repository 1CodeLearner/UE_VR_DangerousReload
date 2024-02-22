// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../VRInteractInterface.h"
#include "VRInteractableActor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class DANGEROUSRELOAD_API AVRInteractableActor : public AActor, public IVRInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVRInteractableActor();
	virtual void OnPickup(AActor* InstigatorA) override;
	virtual void OnRelease(AActor* InstigatorA) override;
	virtual void OnInteract(AActor* InstigatorA) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Interactable")
	TObjectPtr<UBoxComponent> GrabbableBoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Interactable")
	TObjectPtr<USkeletalMeshComponent> SKMComp;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
