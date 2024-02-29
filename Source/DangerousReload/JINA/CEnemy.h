// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CEnemy.generated.h"

UCLASS()
class DANGEROUSRELOAD_API ACEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MySettings")
	class USphereComponent* sphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySettings")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySettings")
	class USkeletalMeshComponent* rightComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySettings")
	class USkeletalMeshComponent* leftComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySettings")
	class UStaticMeshComponent* backComp;

	class ADVRGameModeBase* gameMode;
	class AVRInteractableActor_Pistol* gun;
	float currBulletCount;
	float fakeBulletCount;
	float life = 4;
	class AVRInteractableActor* currentObject;
	class ACharacter* player;
	FVector faceLocation;
	bool bIsShot = false;
	FTimerHandle timerHandle;
	bool bTimerRun = false;

	UPROPERTY(VisibleAnywhere, Category = "MySettings")
	class UVRHealthComponent* HealthComp;

public:
	void MoveToGun();
	void ReturnToBody(ACharacter* target);
	void Shoot(ACharacter* target);
	void ShotGun(ACharacter* target);

	UFUNCTION()
	void OnHealthChanged(bool bDamaged, int HealthRemaining);
	UFUNCTION()
	void OnDead();
};
