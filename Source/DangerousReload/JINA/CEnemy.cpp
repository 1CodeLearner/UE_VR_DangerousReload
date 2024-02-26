// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemy.h"
#include <Components/SphereComponent.h>
#include "../DVRGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include "../Justin/VRInteractables/VRInteractableActor_Pistol.h"
#include "../Justin/VRHealthComponent.h"

// Sets default values
ACEnemy::ACEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(sphereComp);
	sphereComp->SetCollisionObjectType(ECC_GameTraceChannel3);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	meshComp->SetRelativeRotation(FRotator(0, -90, 0));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/EngineMeshes/Cylinder.Cylinder'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}

	// right hand
	rightComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right Hand"));
	rightComp->SetupAttachment(RootComponent);
	rightComp->SetRelativeLocation(FVector(70, 20, -10));
	rightComp->SetRelativeRotation(FRotator(0, -90, 0));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempRightHandMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	if (tempRightHandMesh.Succeeded())
	{
		rightComp->SetSkeletalMesh(tempRightHandMesh.Object);
	}

	// left hand
	leftComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left Hand"));
	leftComp->SetupAttachment(RootComponent);
	leftComp->SetRelativeLocation(FVector(70, -20, -10));
	leftComp->SetRelativeRotation(FRotator(0, -90, 0));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempLeftHandMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	if (tempLeftHandMesh.Succeeded())
	{
		leftComp->SetSkeletalMesh(tempLeftHandMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("/Script/Engine.Material'/Engine/ArtTools/RenderToTexture/Materials/Debug/M_BaseColor_Constant.M_BaseColor_Constant'"));
	if (tempMat.Succeeded())
	{
		meshComp->SetMaterial(0, tempMat.Object);
		rightComp->SetMaterial(0, tempMat.Object);
		leftComp->SetMaterial(0, tempMat.Object);
	}

	HealthComp = CreateDefaultSubobject<UVRHealthComponent>("HealthComp");
}

// Called when the game starts or when spawned
void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
	gameMode = GetWorld()->GetAuthGameMode<ADVRGameModeBase>();
	currBulletCount = gameMode->bulletCount;
	fakeBulletCount = currBulletCount / 2;
	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	gun = Cast<AVRInteractableActor_Pistol>(UGameplayStatics::GetActorOfClass(GetWorld(), AVRInteractableActor_Pistol::StaticClass()));

	HealthComp->OnHealthChanged.AddUObject(this, &ACEnemy::OnHealthChanged);
	HealthComp->OnDead.AddUObject(this, &ACEnemy::OnDead);
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
			Shoot(this);
		}
		else
		{
			// if have check item
			// use check item
			//	if next bullet is fake
			//		shoot to me
			//		Shoot(this);
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
			Shoot(player);

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

void ACEnemy::MoveToGun()
{
	rightComp->SetWorldLocation(gun->GetActorLocation().GetSafeNormal());
	FHitResult hitInfo;
	FVector pos = rightComp->GetComponentLocation();
	FCollisionObjectQueryParams objectParams;
	//objectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	//params.AddIgnoredActor(table);

	bool bChecked = GetWorld()->SweepSingleByObjectType(hitInfo, pos, pos, FQuat::Identity, objectParams, FCollisionShape::MakeSphere(20), params);

	if (bChecked) {
		currentObject = Cast<AVRInteractableActor_Pistol>(hitInfo.GetActor());
		if (currentObject != nullptr)
		{
			//FAttachmentTransformRules attachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;

			//AttachToComponent(rightComp, attachRules, FName("Gun"));
			//currentObject->OnPickup(rightComp);
		}
	}
}

void ACEnemy::ReturnToBody(ACharacter* target)
{
	rightComp->SetRelativeLocation(FVector(0, 20, 0).GetSafeNormal());
	if (rightComp->GetRelativeLocation() == FVector(0, 20, 0))
	{
		ShotGun(target);
	}
}

void ACEnemy::Shoot(ACharacter* target)
{
	if (currentObject == nullptr)
	{
		MoveToGun();
	}
	else {
		ReturnToBody(target);
	}
}

void ACEnemy::ShotGun(ACharacter* target)
{
	rightComp->SetRelativeRotation(FRotator(target->GetActorRotation().Yaw, target->GetActorRotation().Roll * -1, target->GetActorRotation().Pitch));
	// gun->shoot
}

void ACEnemy::OnHealthChanged(bool bDamaged, int HealthRemaining)
{
	//health 가 변경될때 실행
	UE_LOG(LogTemp, Warning, TEXT("%s: Health Changed"), *GetNameSafe(this));
}

void ACEnemy::OnDead()
{
	UE_LOG(LogTemp, Warning, TEXT("%s: IS DEAD!"), *GetNameSafe(this));
}

