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
class APlayerController;

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

	UPROPERTY(VisibleAnywhere, Category = "VRSettings | Components")
	UTextRenderComponent* LTextComp;

	UPROPERTY(VisibleAnywhere, Category = "VRSettings | Components")
	UTextRenderComponent* RTextComp;

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

	//LeftHand

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<USkeletalMeshComponent>  LHandSKMComp;

	//Inputs
	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputMappingContext> IMC_VRCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputAction> IA_RHandGripPress;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputAction> IA_RHandTriggerPress;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputAction> IA_RThumbAButtonPress;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputAction> IA_RGripAxis;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputAction> IA_RTriggerTouch;
		
	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputAction> IA_RTriggerAxis;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputAction> IA_RThumbTouch;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputAction> IA_LGripAxis;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputAction> IA_LTriggerTouch;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputAction> IA_LTriggerAxis;
	
	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputAction> IA_LThumbTouch;

	//Gameplay Components
	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<UVRHealthComponent> HealthComp;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//Hand animations
	friend class UVRHandAnim;

	void RightHandGripAxis(const FInputActionValue& ActionValue);
	float RightHandGripValue;
	void RightHandTriggerTouch(const FInputActionValue& ActionValue);
	bool RHTriggerTouch;
	void RightHandTriggerAxis(const FInputActionValue& ActionValue);
	float RHTriggerAxis;
	void RightHandThumbTouch(const FInputActionValue& ActionValue);
	bool RHThumbTouch;

	void LeftHandGripAxis(const FInputActionValue& ActionValue);
	float LeftHandGripValue;
	void LeftHandTriggerTouch(const FInputActionValue& ActionValue);
	bool LHTriggerTouch;
	void LeftHandTriggerAxis(const FInputActionValue& ActionValue);
	float LHTriggerAxis;
	void LeftHandThumbTouch(const FInputActionValue& ActionValue);
	bool LHThumbTouch;

protected:
	UFUNCTION()
	void OnRightGrip(const FInputActionValue& Value);

	UFUNCTION()
	void OnRightTrigger(const FInputActionValue& Value);

	//Delegates
	void OnHealthChange(bool bDamaged, int HealthRemaining);
	void OnDead();




private:
	bool bIsGripping;
	UPROPERTY()
	AActor* RInteractingActor;

	UPROPERTY()
	TObjectPtr<APlayerController> PC;

	UFUNCTION()
	void FadeOut();
	UFUNCTION()
	void GameOver();

	void RackPistol();
};
