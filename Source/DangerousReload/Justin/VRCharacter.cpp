// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"

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
	

	RMotionComp = CreateDefaultSubobject<UMotionControllerComponent>("LMotionComp");
	RMotionComp->SetupAttachment(RootComponent);
	RHandSKMComp = CreateDefaultSubobject<USkeletalMeshComponent>("RHandSMComp");
	RHandSKMComp->SetupAttachment(RMotionComp);


	LTextComp = CreateDefaultSubobject<UTextRenderComponent>("LTextComp");
	LTextComp->SetupAttachment(LMotionComp);

	RTextComp =  
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
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

}

