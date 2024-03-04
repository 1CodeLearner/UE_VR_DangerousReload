// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRButtonActor.generated.h"

class AVRGameStateBase;
enum class EMatchState : uint8;
class USkeletalMeshComponent;

UCLASS()
class DANGEROUSRELOAD_API AVRButtonActor : public AActor
{
	GENERATED_BODY()

public:
	AVRButtonActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<USkeletalMeshComponent> ButtonMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	EMatchState MatchStateToChangeTo;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY()
	TObjectPtr<AVRGameStateBase> VRGameState;

	void OnMatchStateChanged(EMatchState MatchState);
	
};
