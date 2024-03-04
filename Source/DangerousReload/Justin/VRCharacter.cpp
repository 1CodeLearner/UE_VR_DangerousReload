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
#include "GameFramework/CharacterMovementComponent.h"
#include "VRInteractables/VRInteractableActor_Pistol.h"
#include "VRHandAnim.h"
#include "EngineUtils.h"
#include "DangerousReload/VRGameStateBase.h"

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

	//Start with VR fingers pointing 
	RHTriggerTouch = true;
	LHTriggerTouch = true;
	RHThumbTouch = true;
	LHThumbTouch = true;

	bIsGripping = false;
	bIsHoldingWeapon = false;
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

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);

	HealthComp->OnHealthChanged.AddUObject(this, &AVRCharacter::OnHealthChange);
	HealthComp->OnDead.AddUObject(this, &AVRCharacter::OnDead);

	auto LeftHandAnim = Cast<UVRHandAnim>(LHandSKMComp->GetAnimInstance());
	if (ensure(LeftHandAnim))
	{
		LeftHandAnim->bMirrored = true;
	}
	auto rightHandAnim = Cast<UVRHandAnim>(RHandSKMComp->GetAnimInstance());
	if (rightHandAnim != nullptr)
	{
		rightHandAnim->bMirrored = false;
	}

	for (TActorIterator<AVRInteractableActor_Pistol> it(GetWorld()); it; ++it)
	{
		if (!Weapon && *it)
		{
			Weapon = *it;
		}
	}

	if (ensure(Weapon))
	{
		Weapon->OnWeaponPickedUp.AddUObject(this, &AVRCharacter::OnWeaponPickedup);
		Weapon->OnWeaponDropped.AddUObject(this, &AVRCharacter::OnWeaponDropped);
	}
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
}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//Pistol Interaction
	EnhancedInput->BindAction(IA_RHandGripPress, ETriggerEvent::Started, this, &AVRCharacter::OnRightGrip);
	EnhancedInput->BindAction(IA_RHandGripPress, ETriggerEvent::Completed, this, &AVRCharacter::OnRightGrip);
	EnhancedInput->BindAction(IA_RHandTriggerPress, ETriggerEvent::Started, this, &AVRCharacter::OnRightTrigger);
	EnhancedInput->BindAction(IA_RHandTriggerPress, ETriggerEvent::Started, this, &AVRCharacter::OnRightTrigger);
	EnhancedInput->BindAction(IA_RThumbAButtonPress, ETriggerEvent::Started, this, &AVRCharacter::RackPistol);

	//Hand Animation 
	EnhancedInput->BindAction(IA_RGripAxis, ETriggerEvent::Started, this, &AVRCharacter::RightHandGripAxis);
	EnhancedInput->BindAction(IA_RGripAxis, ETriggerEvent::Triggered, this, &AVRCharacter::RightHandGripAxis);
	EnhancedInput->BindAction(IA_RGripAxis, ETriggerEvent::Completed, this, &AVRCharacter::RightHandGripAxis);

	EnhancedInput->BindAction(IA_RTriggerAxis, ETriggerEvent::Started, this, &AVRCharacter::RightHandTriggerAxis);
	EnhancedInput->BindAction(IA_RTriggerAxis, ETriggerEvent::Triggered, this, &AVRCharacter::RightHandTriggerAxis);
	EnhancedInput->BindAction(IA_RTriggerAxis, ETriggerEvent::Completed, this, &AVRCharacter::RightHandTriggerAxis);

	EnhancedInput->BindAction(IA_RTriggerTouch, ETriggerEvent::Started, this, &AVRCharacter::RightHandTriggerTouch);
	EnhancedInput->BindAction(IA_RTriggerTouch, ETriggerEvent::Completed, this, &AVRCharacter::RightHandTriggerTouch);

	EnhancedInput->BindAction(IA_RThumbTouch, ETriggerEvent::Started, this, &AVRCharacter::RightHandThumbTouch);
	EnhancedInput->BindAction(IA_RThumbTouch, ETriggerEvent::Completed, this, &AVRCharacter::RightHandThumbTouch);

	EnhancedInput->BindAction(IA_LGripAxis, ETriggerEvent::Started, this, &AVRCharacter::LeftHandGripAxis);
	EnhancedInput->BindAction(IA_LGripAxis, ETriggerEvent::Triggered, this, &AVRCharacter::LeftHandGripAxis);
	EnhancedInput->BindAction(IA_LGripAxis, ETriggerEvent::Completed, this, &AVRCharacter::LeftHandGripAxis);

	EnhancedInput->BindAction(IA_LTriggerAxis, ETriggerEvent::Started, this, &AVRCharacter::LeftHandTriggerAxis);
	EnhancedInput->BindAction(IA_LTriggerAxis, ETriggerEvent::Triggered, this, &AVRCharacter::LeftHandTriggerAxis);
	EnhancedInput->BindAction(IA_LTriggerAxis, ETriggerEvent::Completed, this, &AVRCharacter::LeftHandTriggerAxis);

	EnhancedInput->BindAction(IA_LTriggerTouch, ETriggerEvent::Started, this, &AVRCharacter::LeftHandTriggerTouch);
	EnhancedInput->BindAction(IA_LTriggerTouch, ETriggerEvent::Completed, this, &AVRCharacter::LeftHandTriggerTouch);

	EnhancedInput->BindAction(IA_LThumbTouch, ETriggerEvent::Started, this, &AVRCharacter::LeftHandThumbTouch);
	EnhancedInput->BindAction(IA_LThumbTouch, ETriggerEvent::Completed, this, &AVRCharacter::LeftHandThumbTouch);
}

void AVRCharacter::RightHandGripAxis(const FInputActionValue& ActionValue)
{
	RightHandGripValue = ActionValue.Get<float>();
}

void AVRCharacter::LeftHandGripAxis(const FInputActionValue& ActionValue)
{
	LeftHandGripValue = ActionValue.Get<float>();
}

void AVRCharacter::RightHandTriggerTouch(const FInputActionValue& ActionValue)
{
	RHTriggerTouch = !ActionValue.Get<bool>();
}

void AVRCharacter::RightHandTriggerAxis(const FInputActionValue& ActionValue)
{
	RHTriggerAxis = ActionValue.Get<float>();
}

void AVRCharacter::RightHandThumbTouch(const FInputActionValue& ActionValue)
{
	RHThumbTouch = !ActionValue.Get<bool>();
}

void AVRCharacter::LeftHandTriggerTouch(const FInputActionValue& ActionValue)
{
	LHTriggerTouch = !ActionValue.Get<bool>();
}

void AVRCharacter::LeftHandTriggerAxis(const FInputActionValue& ActionValue)
{
	LHTriggerAxis = ActionValue.Get<float>();
}

void AVRCharacter::LeftHandThumbTouch(const FInputActionValue& ActionValue)
{
	LHThumbTouch = !ActionValue.Get<bool>();
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

		/*
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &AVRCharacter::GameOver, 2.f, false);*/
		//auto GameState = Cast<AVRGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
		//if (GameState) 
		//{
		//	GameState->ChangeMatchStateTo(EMatchState::EMATCH_Stop);

		//}
	}
}

void AVRCharacter::RackPistol()
{
	if (ensure(Weapon))
	{
		Weapon->RackPistol();
	}
}

void AVRCharacter::OnWeaponPickedup()
{
	auto VRGameState = GetWorld()->GetGameState<AVRGameStateBase>();
	if (VRGameState && VRGameState->IsCurrentTurn(this))
	{
		bIsHoldingWeapon = true;
	}
}

void AVRCharacter::OnWeaponDropped()
{
	bIsHoldingWeapon = false;
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

