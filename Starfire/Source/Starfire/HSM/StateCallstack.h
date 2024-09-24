// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseState.h"
#include "ECallIInput.h"
#include "Starfire/Utility/BetterObject.h"
#include "StateCallstack.generated.h"



/**
 * 
 */
UCLASS(Blueprintable,ClassGroup=(StateMachine))
class STARFIRE_API UStateCallstack : public UBetterObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<UBaseState*> ActiveStatesByPriority;

public:
	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn=true),Category = "StateMachine")
	FSoftObjectPath StateDefintions;

	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn=true),Category = "StateMachine")
	ACharacter* OwningCharacter;

public:

	//Manage States
	UFUNCTION(BlueprintCallable)
	bool TryAddState(TSubclassOf<UBaseState> BaseStateClass);

	UFUNCTION(BlueprintCallable)
	bool TryRemoveState(TSubclassOf<UBaseState> BaseStateClass);
	
	UFUNCTION(BlueprintCallable)
	void SwitchState(TSubclassOf<UBaseState> StateToAdd, TSubclassOf<UBaseState> StateToRemove);

	UFUNCTION(BlueprintCallable)
	TArray<UBaseState*> GetActiveStates();
	
	//Run Actions
	UFUNCTION(BlueprintCallable)
	void RunCallStack(TSubclassOf<UBaseStateFeature> FeatureClassToRun, ECallInput CallInput,FStateModuleDataStruct Data);


	//CPP Utility Functions
	UBaseStateFeature* GetActiveFeature(TSubclassOf<UBaseStateFeature> FeatureClassToRun);

	void RunActiveStateFeatures(UBaseState* StateToRunOn,ECallInput CallInput, const FStateModuleDataStruct& Data);

	UBaseState* GetStateByClass(TSubclassOf<UBaseState> ClassToSearchFor);

private:


	
};

