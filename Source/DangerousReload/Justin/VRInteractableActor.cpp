// Fill out your copyright notice in the Description page of Project Settings.


#include "VRInteractableActor.h"
#include "VRInteractableActor.h"
#include "VRCharacter.h"
#include "Components/BoxComponent.h"


static TAutoConsoleVariable<bool> CVarTestPhysics(TEXT("jk.TogglePhysics"), true, TEXT("Toggle Test Physics for Interactable objects"), ECVF_Cheat);

// Sets default values
AVRInteractableActor::AVRInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GrabbableBoxComponent = CreateDefaultSubobject<UBoxComponent>("GrabbableBoxComp");
	RootComponent = GrabbableBoxComponent;

	GrabbableBoxComponent->SetBoxExtent(FVector(50.0, 50.0, 50.0));
	GrabbableBoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
	GrabbableBoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	GrabbableBoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	GrabbableBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GrabbableBoxComponent->SetSimulatePhysics(true);
	GrabbableBoxComponent->SetEnableGravity(true);

	SMComp = CreateDefaultSubobject<UStaticMeshComponent>("SMComp");
	SMComp->SetupAttachment(RootComponent);

	SMComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SMComp->SetRelativeLocation(FVector(0.0, 0.0, -50));
	SMComp->SetSimulatePhysics(false);
}

void AVRInteractableActor::OnPickup(AActor* InstigatorA)
{
	UE_LOG(LogTemp, Warning, TEXT("OnPickup invoked"));

	bool bIsPhysicsEnabled = false;
	//bIsPhysicsEnabled = CVarTestPhysics.GetValueOnGameThread();
	GrabbableBoxComponent->SetSimulatePhysics(bIsPhysicsEnabled);

	auto VRCharacter = Cast<AVRCharacter>(InstigatorA);
	AttachToComponent(VRCharacter->RHandSKMComp, FAttachmentTransformRules::KeepWorldTransform);
}




void AVRInteractableActor::OnRelease(AActor* InstigatorA)
{
	UE_LOG(LogTemp, Warning, TEXT("OnRelease invoked"));

	FDetachmentTransformRules Rule = FDetachmentTransformRules::KeepWorldTransform;
	DetachFromActor(Rule);
	GrabbableBoxComponent->SetSimulatePhysics(true);
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

