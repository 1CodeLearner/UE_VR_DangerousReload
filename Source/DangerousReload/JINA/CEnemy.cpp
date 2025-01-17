// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemy.h"
#include <Components/SphereComponent.h>
#include "../DVRGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include "../Justin/VRInteractables/VRInteractableActor_Pistol.h"
#include "../Justin/VRHealthComponent.h"
#include "EngineUtils.h"
#include "../VRGameStateBase.h"
#include "../Justin/VRCharacter.h"
#include "CHealthItem.h"

// Sets default values
ACEnemy::ACEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	sphereComp->SetupAttachment(RootComponent);
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
	VRGameState = gameMode->GetGameState<AVRGameStateBase>();

	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	for (TActorIterator<AVRInteractableActor_Pistol> it(GetWorld()); it; ++it)
	{
		gun = *it;
		if (gun != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Gun: %s"), *gun->GetActorNameOrLabel());
			break;
		}
	}

	HealthComp->OnHealthChanged.AddUObject(this, &ACEnemy::OnHealthChanged);
	HealthComp->OnDead.AddUObject(this, &ACEnemy::OnDead);

	faceLocation = meshComp->GetRelativeLocation();
	life = HealthComp->GetMaxHealth();
	VRGameState->OnMatchStateChanged.AddUObject(this, &ACEnemy::OnMatchStateChanged);
}

// Called every frame
void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//dead
	if (HealthComp->GetMaxHealth() <= 0) return;

	// face return
	if (meshComp->GetRelativeLocation().X < faceLocation.X) {
		UE_LOG(LogTemp, Warning, TEXT("Return To Face"));
		meshComp->SetRelativeLocation(meshComp->GetRelativeLocation() + FVector(10, 0, 0));
		return;
	}

	// round change
	if (gun->IsEmpty())
	{
		if (currentObject != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("I Have Gun"));
			rightComp->SetWorldLocation(rightComp->GetComponentLocation() + (player->GetActorLocation() + FVector::ForwardVector * 100 - rightComp->GetComponentLocation()).GetSafeNormal());
			if (FVector::Distance(player->GetActorLocation(), rightComp->GetComponentLocation()) < 200)
			{
				gun->OnRelease(this);
				rightComp->SetRelativeLocation(FVector(70, 20, -10));
				rightComp->SetRelativeRotation(FRotator(0, -90, 0));
				currentObject = nullptr;
			}
			return;
		}
	}

	// Enemy turn
	if (VRGameState->IsCurrentTurn(this) && !Cast<AVRCharacter>(player)->bIsGripping) {
		TArray<ACHealthItem*> items;
		items.Empty();
		for (TActorIterator<ACHealthItem> it(GetWorld()); it; ++it)
		{
			items.Add(*it);
		}
		if (!items.IsEmpty() && HealthComp->GetMaxHealth() < 4 && bIsShot == false)
		{
			if (healthItem == nullptr) {
				for (int32 i = 0; i < items.Num(); ++i) {
					if (FVector::Distance(GetActorLocation(), items[i]->GetActorLocation()) < 300)
					{
						healthItem = items[i];
						break;
					}
				}
			}
			else {
				rightComp->SetWorldLocation(rightComp->GetComponentLocation() + (healthItem->GetActorLocation() - rightComp->GetComponentLocation()).GetSafeNormal());
				if (FVector::Distance(rightComp->GetComponentLocation(), healthItem->GetActorLocation()) < 30) {
					healthItem->OnPickup(this);
					healthItem->OnInteract(this);
					healthItem = nullptr;
				}
				return;
			}
		}

		float failedPercent = trueBulletCount / gun->GetRemainingRounds() * 100.f;
		if (failedPercent <= 20)
		{
			// shoot to me
			if (bIsShot == true) {
				if (bTimerRun == false) {
					bTimerRun = true;
					GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&]()
						{
							bIsShot = false;
							bTimerRun = false;
						}), 1.5f, false);
				}
				else return;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Shot to me"));
				Shoot(this);
			}
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
		}
	}
	// player turn
	else if (VRGameState->IsCurrentTurn(player))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerTurn"));
		//gun release
		TArray<AActor*> isChild;
		if (rightComp->GetChildComponent(0) || rightComp->GetChildComponent(1))
		{
			UE_LOG(LogTemp, Warning, TEXT("I Have Gun"));
			rightComp->SetWorldLocation(rightComp->GetComponentLocation() + (player->GetActorLocation() + FVector::ForwardVector * 100 - rightComp->GetComponentLocation()).GetSafeNormal());
			if (FVector::Distance(player->GetActorLocation(), rightComp->GetComponentLocation()) < 200)
			{
				gun->OnRelease(this);
				rightComp->SetRelativeLocation(FVector(70, 20, -10));
				rightComp->SetRelativeRotation(FRotator(0, -90, 0));
				currentObject = nullptr;
				bIsShot = false;
			}
			return;
		}
		// if succeed
		
		// if fail
		//

		/*BulletCount is tracked by AVRInteractableActor_Pistol*/
		/*Switching Turns is done by DVRGameModeBase*/
		//else if (gameMode->CurrentTurn = Cast<AActor>(this))
		//{
		//	currBulletCount -= 1;
		//	fakeBulletCount -= 1;
		//	gameMode->isPlayerTurn = false;
		//}
	}
	if (HealthComp->GetMaxHealth() != life) {
		OnHealthChanged(true, HealthComp->GetMaxHealth());
		life = HealthComp->GetMaxHealth();
	}
}

// Called to bind functionality to input
void ACEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACEnemy::OnMatchStateChanged(EMatchState MatchState)
{
	//Game flow logic for Enemy here
	switch (MatchState)
	{
	case EMatchState::EMATCH_Start:
	{
		trueBulletCount = gun->GetLiveRounds();
		break;
	}
	case EMatchState::EMATCH_SwitchTurn:
	{
		break;
	}
	case EMatchState::EMATCH_RoundReset:
	{
		trueBulletCount = gun->GetLiveRounds();
		break;
	}
	//etc.
	}
}

void ACEnemy::MoveToGun()
{
	if (gun != nullptr) 
	rightComp->SetWorldLocation(
	rightComp->GetComponentLocation() + 
	(gun->GetActorLocation() - rightComp->GetComponentLocation()).GetSafeNormal() * 4.f);
	
	if (gun != nullptr) {
		FVector gunPos = gun->GetActorLocation();
		FVector handPos = rightComp->GetComponentLocation();

		if (FVector::Distance(gunPos, handPos) < 20)
		{
			if (currentObject == nullptr)
			{
				currentObject = gun;
				currentObject->OnPickup(this);
				bIsShot = false;
			}
		}
	}
}

void ACEnemy::ReturnToBody(ACharacter* target)
{
	if (FVector::Distance(this->GetActorLocation(), rightComp->GetComponentLocation()) < 100)
	{
		ShotGun(target);
	}
	else {
		rightComp->SetWorldLocation(rightComp->GetComponentLocation() + (this->GetActorLocation() + FVector(70, 20, -10) - rightComp->GetComponentLocation()).GetSafeNormal() * 3.f);
	}
}


void ACEnemy::Shoot(ACharacter* target)
{
	if (currentObject == nullptr)
	{
		MoveToGun();
	}
	else if (bIsShot == false) {
		ReturnToBody(target);
	}
}

void ACEnemy::ShotGun(ACharacter* target)
{
	rightComp->SetRelativeRotation(FRotator(target->GetActorRotation().Pitch, target->GetActorRotation().Yaw * -1, target->GetActorRotation().Roll));
	gun->SetActorRelativeRotation(rightComp->GetComponentRotation());
	/*gun->shoot*/

	gun->SetActorInLOS(target);

	if (!gun->IsRacked())
	{
		gun->RackPistol();
	}

	if (bIsShot == false) {
		gun->OnInteract(target);
		bIsShot = true;
	}
}

void ACEnemy::OnHealthChanged(bool bDamaged, int HealthRemaining)
{
	//health 가 변경될때 실행
	UE_LOG(LogTemp, Warning, TEXT("%s: Health Changed"), *GetNameSafe(this));
	if (bDamaged) {
		meshComp->SetRelativeLocation(meshComp->GetRelativeLocation() - FVector(500, 0, 0));
		trueBulletCount -= 1;
		if (HealthComp->IsDead())
		{
			OnDead();
		}
	}
}

void ACEnemy::OnDead()
{
	UE_LOG(LogTemp, Warning, TEXT("%s: IS DEAD!"), *GetNameSafe(this));
	meshComp->SetRelativeLocation(FVector(-70, 0, 0));
	rightComp->SetRelativeLocation(FVector(-70, 0, 0));
	leftComp->SetRelativeLocation(FVector(-70, 0, 0));
}

