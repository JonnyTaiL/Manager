// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HTTPModule.h"

#include "W_SimWindowBody.generated.h"

/**
 * 
 */
UCLASS()
class MANAGER_API UW_SimWindowBody : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct();
public:



	// Variables //

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> SimArrayIds;
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> CompletedSimArrayIds;


	UPROPERTY(BlueprintReadWrite)
	TMap<FString, FString> SimVariantIds;



	// FUNCTIONS //
	UFUNCTION(BlueprintCallable)
	void GetAllUsVariantsSend();
	void GetAllUsVariantsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);




};
