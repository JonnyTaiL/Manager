// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Scrollbox.h"
#include "Manager/PandaOS/Tests/W_TestsSlot.h"
#include "HTTPModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Manager/ManagerTypes.h"

#include "W_TestsWindowBody.generated.h"

/**
 * 
 */
UCLASS()
class MANAGER_API UW_TestsWindowBody : public UUserWidget
{
	GENERATED_BODY()


	virtual void NativeConstruct();

public:

	// WIDGETS BINDING //

	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* SCROLL_TestsVariants;





	// VARIABLES //

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> TestsArrayIds;

	UPROPERTY(BlueprintReadWrite)
	TArray<FTestDataArrayStruct> TestDataArray;





	// FUNCTIONS //

	UFUNCTION(BlueprintCallable)
	void GetVariantsDataSend();
	void GetVariantsDataReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void GetTestDataArraySend(FString Id);
	void GetTestDataArrayRecieve(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void AddCompletedVariantSend(int32 UserId, int32 VarID, int32 Score);
	void AddCompletedVariantRecieve(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	
};
