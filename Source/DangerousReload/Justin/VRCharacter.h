// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "MySettings | Components")
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(EditDefaultsOnly, Category = "MySettings | Components")
	TObjectPtr<UStaticMeshComponent> HeadSMComp;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings | Components")
	TObjectPtr<UMotionControllerComponent> LMotionComp;	
	UPROPERTY(EditDefaultsOnly, Category = "MySettings | Components")
	TObjectPtr<UMotionControllerComponent> RMotionComp;
	UPROPERTY(EditDefaultsOnly, Category = "MySettings | Components")
	TObjectPtr<USkeletalMeshComponent>  RHandSKMComp;
	UPROPERTY(EditDefaultsOnly, Category = "MySettings | Components")
	TObjectPtr<USkeletalMeshComponent>  LHandSKMComp;

	UPROPERTY(VisibleAnywhere, Category = "MySettings | Components")
	UTextRenderComponent* LTextComp;
	UPROPERTY(VisibleAnywhere, Category = "MySettings | Components")
	UTextRenderComponent* RTextComp;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
