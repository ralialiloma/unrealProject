﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_Equipment.h"

#include "Engine/SkeletalMeshSocket.h"
#include "Starfire/Weapon/FireBlocks.h"
#include "Starfire/Weapon/WeaponOwner.h"
#include "Starfire/Utility/InputSignalType.h"

DEFINE_LOG_CATEGORY_STATIC(EquipmentComponent, Log, All);

// Sets default values for this component's properties
USF_Equipment::USF_Equipment(): EquippedWeapon(nullptr), CurrentEquipmentFlags(0)
{
	PrimaryComponentTick.bCanEverTick = true;
	Mobility = EComponentMobility::Type::Movable;
}

void USF_Equipment::InitializeComponent()
{
	Super::InitializeComponent();
	
	if (!GetOwner()->Implements<UWeaponOwner>())
		UE_LOG(EquipmentComponent, Error, TEXT("Actor requires interface %s"),*UWeaponOwner::StaticClass()->GetName());

	EquippedWeapon = nullptr;
}

void USF_Equipment::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property == nullptr)
		return;

	//Attach With New Socket
	FName PropertyName = PropertyChangedEvent.Property->GetFName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(USF_Equipment, WeaponAttachmentSocket))
		AttachToParentMesh();

}

// Called when the game starts
void USF_Equipment::BeginPlay()
{
	Super::BeginPlay();
}

bool USF_Equipment::IsEquipped() const
{
	return IsValid(EquippedWeapon);
}

bool USF_Equipment::IsAiming() const
{
	if (!IsEquipped())
		return false;
	
	return EquippedWeapon->IsAiming();
}

FWeaponAnimData USF_Equipment::GetEquippedAnimationData() const
{
	if (!IsEquipped())
		return FWeaponAnimData();

	return EquippedWeapon->GetActiveConfig().GetAnimData();
}

AWeaponBase* USF_Equipment::GetActiveWeapon() const
{
	return EquippedWeapon;
}

void USF_Equipment::AddWeapon(AWeaponBase* WeaponToAdd, const bool Equip, int &Index)
{
	//If Weapon Is Already Equipped
	int FoundWeaponIndex = -1;
	if (GetSlotByWeapon(WeaponToAdd,FoundWeaponIndex))
	{
		UE_LOG(EquipmentComponent, Log, TEXT("Weapon Already Equipped"));
		Index = FoundWeaponIndex;
		return;
	}

	if (!IsValid(WeaponToAdd))
	{
		UE_LOG(EquipmentComponent, Error, TEXT("The Weapon you're trying to equip is invalid"));
		return;
	}

	//Add Weapon To List
	OwnedWeapons.Add(WeaponToAdd);
	Index =  OwnedWeapons.Find(WeaponToAdd);

	//Attach Weapon
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules (
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true);
	WeaponToAdd->AttachToComponent(this, AttachRules, "None");

	if (Equip)
	{
		EquippedWeapon = WeaponToAdd;
		EquippedWeapon->OnEquip(GetOwner());
		UE_LOG(EquipmentComponent, Log, TEXT("Equipped Weapon: %s"),*EquippedWeapon->GetName());
	}



	//todo SetWeaponActive
	//todo call pickup event
	
}

void USF_Equipment::AddWeaponByClass(TSubclassOf<AWeaponBase> ActorClass, bool Equip, int& Index)
{
	AActor* SpawnedActor =  GetWorld()->SpawnActor(ActorClass);
	AWeaponBase* WeaponBase = Cast<AWeaponBase>(SpawnedActor);
	AddWeapon(WeaponBase,Equip,Index);
}

bool USF_Equipment::Fire(EInputSignalType InputSignal, EFireType FireType, FHitResult& OutHitResult, TEnumAsByte<EFireBlock>& OutFireBlock)
{
	if (!IsEquipped())
	{
		OutFireBlock = EFireBlock::NoWeapon;
		return false;
	}
	
	return EquippedWeapon->Fire(InputSignal,FireType,OutHitResult,OutFireBlock);
}


bool USF_Equipment::Reload()
{
	if (!IsEquipped())
		return false;

	return EquippedWeapon->Reload();
}

void USF_Equipment::StopReloading()
{
	if (!IsEquipped())
		return;

	return EquippedWeapon->StopReloading();
}

bool USF_Equipment::IsReloading() const
{
	if (!IsEquipped())
		return false;

	return  EquippedWeapon->IsReloading();
}

bool USF_Equipment::IsMeleeOnCooldown() const
{
	if (!IsEquipped())
		return false;

	return EquippedWeapon->IsInMeleeCooldown();
}

bool USF_Equipment::Aim()
{
	if (!IsEquipped())
		return false;

	EquippedWeapon->AimDownSight();
	return true;
}

void USF_Equipment::StopAiming()
{
	EquippedWeapon->StopAiming();
}

bool USF_Equipment::IsFireOnCooldown() const
{
	if (!IsEquipped())
		return false;
	
	return EquippedWeapon->IsInFireCooldown();
}

bool USF_Equipment::Melee()
{
	if (!IsEquipped())
		return false;

	return EquippedWeapon->Melee();
}


// Called every frame
void USF_Equipment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const int NewFlags = GetCompressedFlags();
	if (NewFlags != CurrentEquipmentFlags)
		OnEquipmentFlagsChange.Broadcast(CurrentEquipmentFlags, NewFlags);
	CurrentEquipmentFlags = NewFlags;
}


bool USF_Equipment::CanMelee() const
{
	if (!IsEquipped())
		return false;

	return EquippedWeapon->CanMelee();
}


int USF_Equipment::GetCompressedFlags() const
{
	int EquipmentFlags = 0;

	USf_FunctionLibrary::SetBit(IsEquipped(),EquipmentFlags,EEquipmentFlags::EquipmentState_Equipped);
	USf_FunctionLibrary::SetBit(IsFireOnCooldown(),EquipmentFlags,EEquipmentFlags::EquipmentState_FireCooldown);
	USf_FunctionLibrary::SetBit(IsAiming(),EquipmentFlags,EEquipmentFlags::EquipmentState_Aiming);
	USf_FunctionLibrary::SetBit(IsReloading(),EquipmentFlags,EEquipmentFlags::EquipmentState_Reloading);
	USf_FunctionLibrary::SetBit(IsMeleeOnCooldown(),EquipmentFlags,EEquipmentFlags::EquipmentState_MeleeCooldown);

	return EquipmentFlags;
}

bool USF_Equipment::CheckFlag(EEquipmentFlags EquipmentFlag) const
{
	return USf_FunctionLibrary::CheckBitFlag(GetCompressedFlags(),EquipmentFlag);
}

bool USF_Equipment::CheckFlagForState(EEquipmentFlags EquipmentFlag, int StateToCheck) const
{
	return USf_FunctionLibrary::CheckBitFlag(StateToCheck,EquipmentFlag);
}

bool USF_Equipment::GetSlotByWeapon(AWeaponBase* WeaponBase, int& OutIndex) const
{
	OutIndex = -1;
	if (!IsValid(WeaponBase))
		return false;

	OutIndex = OwnedWeapons.Find(WeaponBase);
	return OwnedWeapons.Contains(WeaponBase);
}

bool USF_Equipment::GetWeaponBySlot(int Index, AWeaponBase*& OutWeaponBase) const
{
	OutWeaponBase = nullptr;
	if (Index < 0 || Index >= OwnedWeapons.Num())
		return false;

	OutWeaponBase = OwnedWeapons[Index];
	return true;
}

void USF_Equipment::AttachToParentMesh()
{
	USceneComponent* Parent =  GetAttachParent();
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules (
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true);
	AttachToComponent(Parent,AttachRules,WeaponAttachmentSocket);
}

TArray<FName> USF_Equipment::GetWeaponAttachmentSocketOptions() const
{
	USceneComponent* Parent =  GetAttachParent();
	USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(Parent);
	if (!IsValid(SkeletalMeshComponent))
		return TArray<FName>{"None"};

	USkeletalMesh* MeshAsset =  SkeletalMeshComponent->GetSkeletalMeshAsset();
	if (!IsValid(MeshAsset))
		return TArray<FName>{"None"};
	
	TArray<FName> AllSocketNames{};
	const TArray<USkeletalMeshSocket*> AllSockets = SkeletalMeshComponent->GetSkeletalMeshAsset()->GetActiveSocketList();

	for (int32 SocketIdx = 0; SocketIdx < AllSockets.Num(); ++SocketIdx)
		AllSocketNames.Add(AllSockets[SocketIdx]->SocketName);
	
	return AllSocketNames;
}

