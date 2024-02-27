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

	// background
	backComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Back Ground"));
	backComp->SetupAttachment(RootComponent);
	backComp->SetRelativeLocation(FVector(-50, 0, 0));
	backComp->SetWorldScale3D(FVector(0.1f, 5, 3));
	backComp->SetStaticMesh(tempMesh.Object);

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("/Script/Engine.Material'/Engine/ArtTools/RenderToTexture/Materials/Debug/M_BaseColor_Constant.M_BaseColor_Constant'"));
	if (tempMat.Succeeded())
	{
		meshComp->SetMaterial(0, tempMat.Object);
		rightComp->SetMaterial(0, tempMat.Object);
		leftComp->SetMaterial(0, tempMat.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempBlackMat(TEXT("/Script/Engine.Material'/Engine/EngineDebugMaterials/BlackUnlitMaterial.BlackUnlitMaterial'"));
	if (tempBlackMat.Succeeded())
	{
		backComp->SetMaterial(0, tempBlackMat.Object);
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
	for (TObjectIterator<AVRInteractableActor_Pistol> it; it; ++it) {
		gun = Cast<AVRInteractableActor_Pistol>(*it);
		if(gun != nullptr) break;
	}

	HealthComp->OnHealthChanged.AddUObject(this, &ACEnemy::OnHealthChanged);
	HealthComp->OnDead.AddUObject(this, &ACEnemy::OnDead);

	faceLocation = meshComp->GetRelativeLocation();
	life = HealthComp->GetMaxHealth();
}

// Called every frame
void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// face return
	if (meshComp->GetRelativeLocation() != faceLocation) {
		meshComp->SetRelativeLocation(meshComp->GetRelativeLocation() + FVector(1, 0, 0));
		return;
	}

	// Enemy turn
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
	// player turn
	else if (gameMode->isPlayerTurn)
	{
		// if succeed
		if (HealthComp->GetMaxHealth() != life) {
			life = HealthComp->GetMaxHealth();
			OnHealthChanged(true, HealthComp->GetMaxHealth());
		}
		// if fail
		else if (gameMode->CurrentTurn = Cast<AActor>(this))
		{
			currBulletCount -= 1;
			fakeBulletCount -= 1;
			gameMode->isPlayerTurn = false;
		}
	}
}

// Called to bind functionality to input
void ACEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACEnemy::MoveToGun()
{
	if(gun != nullptr) rightComp->SetWorldLocation(rightComp->GetComponentLocation() + (gun->GetActorLocation() - rightComp->GetComponentLocation()).GetSafeNormal());
	if (gun != nullptr) {
		//FHitResult hitInfo;
		//FVector pos = rightComp->GetComponentLocation();
		//FCollisionObjectQueryParams objectParams;
		//objectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
		//FCollisionQueryParams params;
		//params.AddIgnoredActor(this);
		////params.AddIgnoredActor(table);

		//bool bChecked = GetWorld()->SweepSingleByObjectType(hitInfo, pos, pos, FQuat::Identity, objectParams, FCollisionShape::MakeSphere(20), params);
		//DrawDebugSphere(GetWorld(), pos, 20, 10, FColor::Red);
		//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString::Printf(TEXT("bcheck: %d"), bChecked));
		//if (bChecked) {
		//	UE_LOG(LogTemp, Warning, TEXT("33333333333333333333333"));

		//	currentObject = Cast<AVRInteractableActor_Pistol>(hitInfo.GetActor());
		//	if (currentObject != nullptr)
		//	{
		//		//FAttachmentTransformRules attachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;

		//		//AttachToComponent(rightComp, attachRules, FName("Gun"));
		//		currentObject->OnPickup(this);
		//	}
		FVector gunPos = gun->GetActorLocation();
		FVector handPos = rightComp->GetComponentLocation();

		if(FVector::Distance(gunPos, handPos) < 20)
		{
			if (currentObject == nullptr)
			{
				currentObject = gun;
				currentObject->OnPickup(this);
			}
		}
	}
}

void ACEnemy::ReturnToBody(ACharacter* target)
{
	rightComp->SetWorldLocation(rightComp->GetComponentLocation() + (this->GetActorLocation() + FVector(70, 20, -10) - rightComp->GetComponentLocation()).GetSafeNormal());
	if (FVector::Distance(this->GetActorLocation(), rightComp->GetComponentLocation()) < 30)
	{
		UE_LOG(LogTemp, Warning, TEXT("Time to shot"));
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
	gun->FindActorInLOS();
	if (gun->ActorInLOS != nullptr)
	{
		gun->OnInteract(target);
	}
}

void ACEnemy::OnHealthChanged(bool bDamaged, int HealthRemaining)
{
	//health 가 변경될때 실행
	UE_LOG(LogTemp, Warning, TEXT("%s: Health Changed"), *GetNameSafe(this));
	if (bDamaged) {
		meshComp->SetRelativeLocation(FVector(-70, 0, 0));
		gameMode->isPlayerTurn = false;
		currBulletCount -= 1;
		if (HealthComp->IsDead())
		{
			OnDead();
		}
	}
}

void ACEnemy::OnDead()
{
	UE_LOG(LogTemp, Warning, TEXT("%s: IS DEAD!"), *GetNameSafe(this));
	rightComp->SetRelativeLocation(FVector(-70, 0, 0));
	leftComp->SetRelativeLocation(FVector(-70, 0, 0));
}

