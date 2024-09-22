﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Project/Weapon/WeaponOwner.h"
#include "SF_Character.generated.h"

UCLASS(Config = Game)
class PROJECT_API ASf_Character : public ACharacter, public IWeaponOwner
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Movement")
	class USF_CharacterMovementComponent* SFCharacterMovementComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Weapon")
	class USF_Equipment* SFEquipmentComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera")
	class UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Camera")
	class USpringArmComponent* SprintArmComponent;
	
	UPROPERTY(Category="Character", EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

public:
	ASf_Character(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;
	
	FCollisionQueryParams GetIgnoreCharacterParams();

	UFUNCTION(BlueprintPure)
	FORCEINLINE USF_CharacterMovementComponent* GetSfCharacterMovementComponent() const{return SFCharacterMovementComponent;};

	UFUNCTION(BlueprintPure)
	FORCEINLINE UCameraComponent* GetFirstPersonCamera() const{return FirstPersonCamera;};

	UFUNCTION(BlueprintPure)
	FORCEINLINE USkeletalMeshComponent* GetFirstPersonMesh() const{return FirstPersonMesh;};

	UFUNCTION(BlueprintPure)
	FORCEINLINE USF_Equipment* GetSFEquipmentComponent() const{return SFEquipmentComponent;};

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//IWeaponOwner
public:
	virtual UAnimInstance* GetCharacterAnimInstance_Implementation() const override;
	
	virtual UCameraComponent* GetCamera_Implementation() const override;
	
	virtual FTransform GetFireTransform_Implementation() const override;
	
	virtual FMeleeInfo GetMeleeInfo_Implementation() const override;
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponOwner")
	//virtual FMeleeInfo GetMeleeInfo() const;
};
