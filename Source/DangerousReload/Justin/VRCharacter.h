// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "VRCharacter.generated.h"

class UCameraComponent;
class UStaticMeshComponent;
class UMotionControllerComponent;
class USkeletalMeshComponent;
class UTextRenderComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class DANGEROUSRELOAD_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<UStaticMeshComponent> HeadSMComp;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<UMotionControllerComponent> LMotionComp;	
	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<UMotionControllerComponent> RMotionComp;
	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<USkeletalMeshComponent>  RHandSKMComp;
	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Components")
	TObjectPtr<USkeletalMeshComponent>  LHandSKMComp;

	UPROPERTY(VisibleAnywhere, Category = "VRSettings | Components")
	UTextRenderComponent* LTextComp;
	UPROPERTY(VisibleAnywhere, Category = "VRSettings | Components")
	UTextRenderComponent* RTextComp;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputMappingContext> IMC_VRCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "VRSettings | Input")
	TObjectPtr<UInputAction> IA_RHandGripPress;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION()
	void OnRightGrip(const FInputActionValue& Value);
};
