// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemy.h"
#include <Components/SphereComponent.h>
#include "../DVRGameModeBase.h"

// Sets default values
ACEnemy::ACEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(sphereComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/EngineMeshes/Cylinder.Cylinder'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}

	// right hand
	rightComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right Hand"));
	rightComp->SetupAttachment(RootComponent);
	rightComp->SetRelativeLocation(FVector(30, -50, 0));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempRightHandMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	if (tempRightHandMesh.Succeeded())
	{
		rightComp->SetSkeletalMesh(tempRightHandMesh.Object);
	}

	// left hand
	leftComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left Hand"));
	leftComp->SetupAttachment(RootComponent);
	leftComp->SetRelativeLocation(FVector(-30, -50, 0));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempLeftHandMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	if (tempLeftHandMesh.Succeeded())
	{
		leftComp->SetSkeletalMesh(tempLeftHandMesh.Object);
	}


}

// Called when the game starts or when spawned
void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
	currBulletCount = gameMode->bulletCount;
	fakeBulletCount = currBulletCount / 2;

	gameMode = Cast<ADVRGameModeBase>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!gameMode->isPlayerTurn) {
		if (life < 4)
		{
			// use life item during life == 4 or all life item
		}
		float succeedPercent = fakeBulletCount / currBulletCount * 100;
		if (succeedPercent >= 70)
		{
			// shoot to me
		}
		else
		{
			// if have check item
			// use check item
			//	if next bullet is fake
			//		shoot to me
			//	else
			//		shoot to player
			// else if have knife item and player life > 1
			//	if(50%)
			//		use knife item
			// else if succeed percent < 50% and my life < 2
			//		use beer item
			//		if bullet was fake
			//			currBullet - 1 and fakeBullet - 1
			//		else
			//			currBullet - 1
			// else
			//	shoot to player

			// gameMode->isPlayerTurn = true
		}
	}
	else if (gameMode->isPlayerTurn)
	{
		// if fail
		// fakeBulletCount - 1 and currBullet - 1
		// if succeed
		// currBullet - 1

	}
}

// Called to bind functionality to input
void ACEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

