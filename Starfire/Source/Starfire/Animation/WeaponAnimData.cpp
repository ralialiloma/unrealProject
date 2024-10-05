﻿#include "WeaponAnimData.h"

#include "Starfire/Utility/Sf_FunctionLibrary.h"

FWeaponAnimData::FWeaponAnimData()
{
	UpdateEntries();
}

void FWeaponAnimData::UpdateEntries()
{
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EWeaponAnimationAssetType,UAnimSequenceBase*>(AnimationAssets);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EWeaponAnimationMontageType,UAnimMontage*>(AnimationMontages);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EWeaponBlendSpaceType,UBlendSpace*>(Blendspaces);
}

UWeaponAnimationAsset::UWeaponAnimationAsset()
{
	WeaponAnimData.UpdateEntries();
}

void UWeaponAnimationAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	WeaponAnimData.UpdateEntries();
}

UAnimSequenceBase* UWeaponAnimDataFunctions::GetAnimationAsset(FWeaponAnimData AnimData,
                                                               EWeaponAnimationAssetType AssetType)
{
	UAnimSequenceBase* FoundSequence = nullptr;
	UAnimSequenceBase** FoundSequencePtr = AnimData.AnimationAssets.Find(AssetType);
	if (FoundSequencePtr!=nullptr)
		FoundSequence = *FoundSequencePtr;
	return FoundSequence;
}

UAnimMontage* UWeaponAnimDataFunctions::GetAnimationMontage(FWeaponAnimData AnimData,
	EWeaponAnimationMontageType AssetType)
{
	UAnimMontage* FoundMontage = nullptr;
	UAnimMontage** FoundSequencePtr = AnimData.AnimationMontages.Find(AssetType);
	if (FoundSequencePtr!=nullptr)
		FoundMontage = *FoundSequencePtr;
	return FoundMontage;
}

UBlendSpace* UWeaponAnimDataFunctions::GetBlendspace(FWeaponAnimData AnimData, EWeaponBlendSpaceType AssetType)
{
	UBlendSpace* FoundBlendspace = nullptr;
	UBlendSpace** FoundBlendSpacePointer = AnimData.Blendspaces.Find(AssetType);
	if (FoundBlendSpacePointer!=nullptr)
		FoundBlendspace = *FoundBlendSpacePointer;
	return FoundBlendspace;
}

