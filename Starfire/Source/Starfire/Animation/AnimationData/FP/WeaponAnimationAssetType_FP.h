﻿#pragma once

UENUM(Blueprintable)
enum class EWeaponAnimationAssetType_FP: uint8
{
	None = 0 UMETA(Hidden),
	Idle= 2 ,
	JumpStart= 3 ,
	JumpEnd= 4,
	JumpLoop= 5,
};
