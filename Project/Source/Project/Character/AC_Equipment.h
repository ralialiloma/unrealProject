﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Project/Weapon/WeaponBase.h"
#include "AC_Equipment.generated.h"


UCLASS(ClassGroup=(Character), meta=(BlueprintSpawnableComponent))
class PROJECT_API UAC_Equipment : public USceneComponent
{
	GENERATED_BODY()

public:
	UAC_Equipment();

private:
	//Transient
	UPROPERTY()
	TArray<AWeaponBase*> OwnedWeapons;
	UPROPERTY()
	AWeaponBase* EquippedWeapon;

protected:

	//Actor
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

	//Interface
public:
	UFUNCTION(BlueprintCallable)
	bool GetIsEquipped() const;

	UFUNCTION(BlueprintCallable)
	void AddWeapon(AWeaponBase* WeaponToAdd, bool Equip, int& Index);

	//Internal
private:
	bool GetSlot(AWeaponBase* WeaponBase,int& OutIndex) const;
};
