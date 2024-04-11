// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageReceiver.h"

// Sets default values for this component's properties
UDamageReceiver::UDamageReceiver()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UDamageReceiver::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

float UDamageReceiver::CalculateDamageMultiplier(FVector AreaLocation, FVector HitPosition, float DamageMultiplier, float MaxDistanceToBone)
{
	float DistanceFromHitLocation = FVector::Distance(HitPosition,AreaLocation);

	if (DistanceFromHitLocation<=MaxDistanceToBone)
		return DamageMultiplier;
	else
		return 1;
}




// Called every frame
void UDamageReceiver::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

