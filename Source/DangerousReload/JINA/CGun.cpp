// Fill out your copyright notice in the Description page of Project Settings.


#include "CGun.h"
#include <GameFramework/Character.h>
#include <Kismet/GameplayStatics.h>
#include "../DVRGameModeBase.h"

ACGun::ACGun()
{
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempParticle(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (tempParticle.Succeeded())
	{
		explosion = tempParticle.Object;
	}
}

void ACGun::Shoot(ACharacter* target)
{
	// target->life -= 1;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosion, FVector(0, 0, 0));
	gameMode->bulletCount -= 1;
}
