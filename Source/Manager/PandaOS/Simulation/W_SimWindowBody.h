// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HTTPModule.h"
#include "Manager/ManagerTypes.h"

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

	UPROPERTY(BlueprintReadWrite)
	FSimVariantData SimVariantData;


	// FUNCTIONS //
	UFUNCTION(BlueprintCallable)
	void GetAllUsVariantsSend();
	void GetAllUsVariantsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void CreateEmptyUsVariantSend(FString m_VariantName, FString m_Group);
	void CreateEmptyUsVariantRecive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	UFUNCTION(BlueprintCallable)
	void DeleteVariantSend(int32 m_Var_ID);
	void DeleteVariantRecive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	UFUNCTION(BlueprintCallable)
	void GetVariantDataSend(int32 m_Var_ID);
	void GetVariantDataReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


};
