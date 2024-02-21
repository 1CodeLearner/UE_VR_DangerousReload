// Fill out your copyright notice in the Description page of Project Settings.


#include "VRInteractableActor.h"
#include "VRInteractableActor.h"
#include "VRCharacter.h"


static TAutoConsoleVariable<bool> CVarTestPhysics(TEXT("jk.TogglePhysics"), true, TEXT("Toggle Test Physics for Interactable objects"), ECVF_Cheat);

// Sets default values
AVRInteractableActor::AVRInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*GrabbableComponent = CreateDefaultSubobject<USceneComponent>("GrabbableSceneComp");
	GrabbableComponent->SetupAttachment(RootComponent);*/


	SMComp = CreateDefaultSubobject<UStaticMeshComponent>("SMComp");
	RootComponent = SMComp;

}

void AVRInteractableActor::OnPickup(AActor* InstigatorA)
{
	UE_LOG(LogTemp, Warning, TEXT("OnPickup invoked"));
	auto VRCharacter = Cast<AVRCharacter>(InstigatorA);
	SMComp->AttachToComponent(Cast<USceneComponent>(VRCharacter->RMotionComp), FAttachmentTransformRules::KeepWorldTransform);

	bool bIsPhysicsEnabled = false;
	//bIsPhysicsEnabled = CVarTestPhysics.GetValueOnGameThread();

	SMComp->SetSimulatePhysics(bIsPhysicsEnabled);
}




void AVRInteractableActor::OnRelease(AActor* InstigatorA)
{
	UE_LOG(LogTemp, Warning, TEXT("OnRelease invoked"));

	FDetachmentTransformRules Rule = FDetachmentTransformRules::KeepWorldTransform;
	DetachFromActor(Rule);
	SMComp->SetSimulatePhysics(true);
}

void AVRInteractableActor::OnInteract(AActor* InstigatorA)
{
	UE_LOG(LogTemp, Warning, TEXT("OnInteract invoked"));
}

// Called when the game starts or when spawned
void AVRInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVRInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

