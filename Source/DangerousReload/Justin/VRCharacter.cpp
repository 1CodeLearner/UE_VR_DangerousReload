// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "../VRInteractInterface.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "VRHealthComponent.h"
#include "DangerousReload/DVRGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Head
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(RootComponent);

	HeadSMComp = CreateDefaultSubobject<UStaticMeshComponent>("HeadSMComp");
	HeadSMComp->SetupAttachment(CameraComp);

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetupAttachment(CameraComp);
	SphereComp->SetSphereRadius(12);

	//LeftHand
	LMotionComp = CreateDefaultSubobject<UMotionControllerComponent>("LMotionComp");
	LMotionComp->SetupAttachment(RootComponent);
	LHandSKMComp = CreateDefaultSubobject<USkeletalMeshComponent>("LHandSMComp");
	LHandSKMComp->SetupAttachment(LMotionComp);
	LHandSKMComp->SetCollisionObjectType(ECC_GameTraceChannel2);


	LTextComp = CreateDefaultSubobject<UTextRenderComponent>("LTextComp");
	LTextComp->SetupAttachment(LMotionComp);

	//RightHand
	RMotionComp = CreateDefaultSubobject<UMotionControllerComponent>("RMotionComp");
	RMotionComp->SetupAttachment(RootComponent);
	RHandSKMComp = CreateDefaultSubobject<USkeletalMeshComponent>("RHandSMComp");
	RHandSKMComp->SetupAttachment(RMotionComp);
	RHandSKMComp->SetCollisionObjectType(ECC_GameTraceChannel2);

	RTextComp = CreateDefaultSubobject<UTextRenderComponent>("RTextComp");
	RTextComp->SetupAttachment(RMotionComp);

	//Right hand grip
	RCollisionComp = CreateDefaultSubobject<USphereComponent>("RInteractSphereComp");
	RCollisionComp->SetupAttachment(RMotionComp);
	bIsGripping = false;

	auto Capsule = GetCapsuleComponent();
	Capsule->SetCollisionObjectType(ECC_GameTraceChannel3);
	GetCharacterMovement()->GravityScale = 0.f;

	HealthComp = CreateDefaultSubobject<UVRHealthComponent>("HealthComp");
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();

	PC = GetController<APlayerController>();

	if (ensure(PC))
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedInput = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

		if (ensure(IMC_VRCharacter))
		{
			EnhancedInput->AddMappingContext(IMC_VRCharacter, 0);
		}
	}

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

	HealthComp->OnHealthChanged.AddUObject(this, &AVRCharacter::OnHealthChange);
	HealthComp->OnDead.AddUObject(this, &AVRCharacter::OnDead);
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If no interacting actor
	if (bIsGripping && !RInteractingActor)
	{
		TArray<AActor*> OverlappingActors;
		RCollisionComp->GetOverlappingActors(OverlappingActors);
		if (!OverlappingActors.IsEmpty())
		{
			AActor* ClosestActor = nullptr;
			//Find the closest Actor
			for (auto Actor : OverlappingActors)
			{
				if (ClosestActor && Actor->Implements<UVRInteractInterface>()) {
					FVector DistVector = RMotionComp->GetComponentLocation() - Actor->GetActorLocation();
					float DistSqrd = FVector::DotProduct(DistVector, DistVector);

					FVector DistVectorClosest = RMotionComp->GetComponentLocation() - ClosestActor->GetActorLocation();
					float DistSqrdClosest = FVector::DotProduct(DistVectorClosest, DistVectorClosest);

					if (DistSqrd < DistSqrdClosest)
					{
						ClosestActor = Actor;
					}
				}
				else if (Actor->Implements<UVRInteractInterface>())
				{
					ClosestActor = Actor;
					auto VRInterface = Cast<IVRInteractInterface>(ClosestActor);
					VRInterface->OnPickup(this);
				}
			}
			if (ClosestActor)
			{
				RInteractingActor = ClosestActor;
			}
		}
	}
	else
	{
		if (RInteractingActor && !bIsGripping)
		{
			auto VRGrabInterface = Cast<IVRInteractInterface>(RInteractingActor);
			if (VRGrabInterface)
				VRGrabInterface->OnRelease(this);
			RInteractingActor = nullptr;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Magenta, FString::Printf(TEXT("InteractingActor: %s"), *GetNameSafe(RInteractingActor)));
}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInput->BindAction(IA_RHandGripPress, ETriggerEvent::Started, this, &AVRCharacter::OnRightGrip);
	EnhancedInput->BindAction(IA_RHandGripPress, ETriggerEvent::Completed, this, &AVRCharacter::OnRightGrip);

	EnhancedInput->BindAction(IA_RHandTriggerPress, ETriggerEvent::Started, this, &AVRCharacter::OnRightTrigger);

	EnhancedInput->BindAction(IA_RHandTriggerPress, ETriggerEvent::Started, this, &AVRCharacter::OnRightTrigger);


	EnhancedInput->BindAction(IA_RThumbAButtonPress, ETriggerEvent::Started, this, &AVRCharacter::RackPistol);
}

void AVRCharacter::OnRightGrip(const FInputActionValue& Value)
{
	bIsGripping = Value.Get<bool>();
}

void AVRCharacter::OnRightTrigger(const FInputActionValue& Value)
{
	if (bIsGripping && RInteractingActor)
	{
		auto temp = Cast<IVRInteractInterface>(RInteractingActor);
		if (temp)
		{
			temp->OnInteract(this);
		}
	}
}

void AVRCharacter::OnHealthChange(bool bDamaged, int HealthRemaining)
{
	if (bDamaged)
	{
		UE_LOG(LogTemp, Warning, TEXT("Damaged: %d"), HealthRemaining);
		if (PC)
		{
			PC->PlayerCameraManager->StartCameraFade(0, 1, .01f, FColor::Black, false, true);
			FTimerHandle Handle;
			GetWorld()->GetTimerManager().SetTimer(Handle, this, &AVRCharacter::FadeOut, 2.f, false);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Recovered: %d"), HealthRemaining);
	}
}

void AVRCharacter::OnDead()
{
	if (PC)
	{
		PC->PlayerCameraManager->StartCameraFade(0, 1, .01f, FColor::Black, false, true);
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &AVRCharacter::GameOver, 2.f, false);
	}
}

void AVRCharacter::RackPistol()
{
	auto GM = GetWorld()->GetAuthGameMode<ADVRGameModeBase>();
	GM->SetCanFire(true);
}

void AVRCharacter::FadeOut()
{
	if (PC)
	{
		PC->PlayerCameraManager->StartCameraFade(1, 0, 1.f, FColor::Black);
	}
}

void AVRCharacter::GameOver()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("Player is Dead")));
}

