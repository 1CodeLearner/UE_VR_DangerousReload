// Fill out your copyright notice in the Description page of Project Settings.


#include "CHealthItem.h"

ACHealthItem::ACHealthItem()
{
	btnComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button Component"));
	btnComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT(""));
	if (tempMesh.Succeeded())
	{
		btnComp->SetStaticMesh(tempMesh.Object);
	}
}

void ACHealthItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if push


}

void ACHealthItem::OnInteract(AActor* InstigatorA)
{
	Super::OnInteract(InstigatorA);
}

