// Fill out your copyright notice in the Description page of Project Settings.


#include "CHealthItem.h"
#include "../Justin/VRCharacter.h"
#include "CEnemy.h"
#include "../Justin/VRHealthComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "../DVRGameModeBase.h"

ACHealthItem::ACHealthItem()
{
	btnComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Button Component"));
	btnComp->SetupAttachment(RootComponent);
	/*ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT(""));
	if (tempMesh.Succeeded())
	{
		btnComp->SetSkeletalMesh(tempMesh.Object);
	}*/
	btnComp->SetRelativeLocation(FVector(0, 0, 30));
}

void ACHealthItem::BeginPlay()
{
	Super::BeginPlay();
}

void ACHealthItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("I Have Owner"));
		player = Cast<AVRCharacter>(GetOwner());
		enemy = Cast<ACEnemy>(GetOwner());
		if (player)
		{
			UE_LOG(LogTemp, Warning, TEXT("Owner is player"));
			btnComp->SetWorldLocation(player->LHandSKMComp->GetComponentLocation());
		}
		else if (enemy)
		{
			UE_LOG(LogTemp, Warning, TEXT("Owner is Enemy"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("I don't have owner"));
	}

	//if push
	if (FVector::Distance(btnComp->GetComponentLocation(), SKMComp->GetComponentLocation()) < 30)
	{
		OnInteract(GetOwner());
	}
	
}

void ACHealthItem::OnPickup(AActor* InstigatorA)
{
	Super::OnPickup(InstigatorA);
}

void ACHealthItem::OnInteract(AActor* InstigatorA)
{
	Super::OnInteract(InstigatorA);
	player = Cast<AVRCharacter>(InstigatorA);
	enemy = Cast<ACEnemy>(InstigatorA);
	if (player)
	{
		player->HealthComp->SetMaxHealth(player->HealthComp->GetMaxHealth() + 1);
		gameMode->ChangeLifeLightColor(player, FLinearColor::White);
	}
	else if (enemy)
	{
		enemy->HealthComp->SetMaxHealth(enemy->HealthComp->GetMaxHealth() + 1);
		gameMode->ChangeLifeLightColor(enemy, FLinearColor::White);
	}

	if (player || enemy)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), btnEffect, GetActorLocation());
		SetOwner(nullptr);
		Destroy();
	}
}

void ACHealthItem::OnRelease(AActor* InstigatorA)
{
	Super::OnRelease(InstigatorA);
}


