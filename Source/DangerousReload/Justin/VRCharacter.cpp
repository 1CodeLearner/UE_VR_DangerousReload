// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/SphereComponent.h"
#include "../VRInteractInterface.h"

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
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto PC = GetController<APlayerController>();

	UEnhancedInputLocalPlayerSubsystem* EnhancedInput = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

	if (ensure(IMC_VRCharacter))
	{
		EnhancedInput->AddMappingContext(IMC_VRCharacter, 0);
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
			UE_LOG(LogTemp, Warning, TEXT("%s(%d) - OnRightTrigger Input"), *FString(__FUNCTION__), __LINE__);
			temp->OnInteract(this);
		}
	}
}

