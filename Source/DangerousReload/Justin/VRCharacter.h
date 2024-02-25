// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "VRCharacter.generated.h"

class UVRHealthComponent;
class UCameraComponent;
class UStaticMeshComponent;
class UMotionControllerComponent;
class USkeletalMeshComponent;
class UTextRenderComponent;
class UInputMappingContext;
class UInputAction;
class USphereComponent;

UCLASS()
class DANGEROUSRELOAD_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();
	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<UMotionControllerComponent> RMotionComp;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<UMotionControllerComponent> LMotionComp;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<USkeletalMeshComponent>  RHandSKMComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Head
	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<UStaticMeshComponent> HeadSMComp;
	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<USphereComponent> SphereComp;


	//RightHand


	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<USphereComponent> RCollisionComp;
	UPROPERTY(VisibleAnywhere, Category = "VRSettings | Components")
	UTextRenderComponent* RTextComp;

	//LeftHand

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<USkeletalMeshComponent>  LHandSKMComp;
	UPROPERTY(VisibleAnywhere, Category = "VRSettings | Components")
	UTextRenderComponent* LTextComp;

	//Inputs
	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputMappingContext> IMC_VRCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputAction> IA_RHandGripPress;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputAction> IA_RHandTriggerPress;


	//Gameplay Components
	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<UVRHealthComponent> HealthComp;

	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION()
	void OnRightGrip(const FInputActionValue& Value);

	UFUNCTION()
	void OnRightTrigger(const FInputActionValue& Value);

	void OnHealthChange(bool bDamaged, int HealthRemaining);

private:
	UPROPERTY()
	AActor* RInteractingActor;
	void CheckGrabObject();
};
