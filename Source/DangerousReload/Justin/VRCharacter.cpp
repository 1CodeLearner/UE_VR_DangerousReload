// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(RootComponent);

	HeadSMComp = CreateDefaultSubobject<UStaticMeshComponent>("HeadSMComp");
	HeadSMComp->SetupAttachment(CameraComp);

	LMotionComp = CreateDefaultSubobject<UMotionControllerComponent>("LMotionComp");
	LMotionComp->SetupAttachment(RootComponent);
	LHandSKMComp = CreateDefaultSubobject<USkeletalMeshComponent>("LHandSMComp");
	LHandSKMComp->SetupAttachment(LMotionComp);

	RMotionComp = CreateDefaultSubobject<UMotionControllerComponent>("RMotionComp");
	RMotionComp->SetupAttachment(RootComponent);
	RHandSKMComp = CreateDefaultSubobject<USkeletalMeshComponent>("RHandSMComp");
	RHandSKMComp->SetupAttachment(RMotionComp);

	LTextComp = CreateDefaultSubobject<UTextRenderComponent>("LTextComp");
	LTextComp->SetupAttachment(LMotionComp);

	RTextComp =  CreateDefaultSubobject<UTextRenderComponent>("RTextComp");
	RTextComp->SetupAttachment(RMotionComp);
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto PC = GetController<APlayerController>();

	UEnhancedInputLocalPlayerSubsystem* EnhancedInput =  ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

	if(ensure(IMC_VRCharacter))
	{
		EnhancedInput->AddMappingContext(IMC_VRCharacter, 0);
	}
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInput->BindAction(IA_RHandGripPress, ETriggerEvent::Started, this, &AVRCharacter::OnRightGrip);
}

void AVRCharacter::OnRightGrip(const FInputActionValue& Value)
{
	FString Output;
	Output = Value.Get<bool>() ? TEXT("Good") : TEXT("Bad");

	RTextComp->SetText(FText::FromString(FString::Printf(TEXT("What"))));
}

