// Fill out your copyright notice in the Description page of Project Settings.


#include "VRHealthComponent.h"

UVRHealthComponent::UVRHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = 1;
	bIsDead = false;
}

void UVRHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVRHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Magenta, FString::Printf(TEXT("Health: %d"), MaxHealth));

}


void UVRHealthComponent::InflictDamage()
{
	//if dead, return
	if (!ensure(!IsDead()))
		return;

	MaxHealth--;
	OnHealthChanged.Broadcast(true, MaxHealth);
}

void UVRHealthComponent::RecoverHealth()
{
	//if dead, return
	if (!ensure(!IsDead()))
		return;

	MaxHealth++;
	OnHealthChanged.Broadcast(false, MaxHealth);
}

void UVRHealthComponent::SetMaxHealth(const int Health)
{
	MaxHealth = Health;
}


bool UVRHealthComponent::IsDead() const
{
	return MaxHealth <= 0;
}

