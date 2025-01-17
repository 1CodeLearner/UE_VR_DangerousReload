// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VRHealthComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FHealthChangedDelegate, bool bDamaged, int HealthRemaining);
DECLARE_MULTICAST_DELEGATE(FDeadDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DANGEROUSRELOAD_API UVRHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UVRHealthComponent();

	void SetMaxHealth(const int MaxHealth);

	void InflictDamage();

	void RecoverHealth();

	bool IsDead() const;

	int GetMaxHealth() const;

	FHealthChangedDelegate OnHealthChanged;
	FDeadDelegate OnDead;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category = "State");
	int MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "State");

	//Set to dead when game over or game won
	bool bIsDead;
};
