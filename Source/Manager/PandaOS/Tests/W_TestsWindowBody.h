// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Scrollbox.h"
#include "Manager/W_TextBox_Default.h"
#include "Manager/PandaOS/Tests/W_TestsSlot.h"
#include "HTTPModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Manager/ManagerTypes.h"
#include "Manager/PandaOS/WindowBodyMaster.h"

#include "W_TestsWindowBody.generated.h"

/**
 * 
 */


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVariantDataReceived, bool, Success);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestDataArrayReceived, bool, Success);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompletedVariantsReceived, bool, Success);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestionReceiced, bool, Success);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllQuestionsReveived, bool, Success);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestionsUpdated, bool, Success);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVariantCreated, bool, Success);


UCLASS()
class MANAGER_API UW_TestsWindowBody : public UWindowBodyMaster
{
	GENERATED_BODY()


	virtual void NativeConstruct();

public:

	// Delegates //
	//UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	//FOnVariantDataReceived FOnVariantDataReceived_Callback;

	//UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	//FOnTestDataArrayReceived FOnTestDataArrayReceived_Callback;

	//UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	//FOnCompletedVariantsReceived FOnCompletedVariantsReceived_Callback;

	//UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	//FOnQuestionReceiced FOnQuestionReceiced_Callback;

	//UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	//FOnAllQuestionsReveived FOnAllQuestionsReveived_Callback;

	//UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	//FOnQuestionsUpdated FOnQuestionsUpdated_Callback;

	//UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	//FOnVariantCreated FOnVariantCreated_Callback;

	// WIDGETS BINDING //

	


	//  Change To Scrollbox !!!!!!!!
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_TextBox_Default* TB_Group;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_TextBox_Default* TB_VarName;




	// VARIABLES //

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> TestsArrayIds;
	UPROPERTY(BlueprintReadWrite)
	TArray<FString> CompletedTestsArrayIds;

	UPROPERTY(BlueprintReadWrite)
	TArray<FTestDataArrayStruct> TestDataArray;


	TArray<FString> GroupsArray;

	UPROPERTY(BlueprintReadWrite)
	TArray<FTestDataArrayStruct> Questions;

	UPROPERTY(BlueprintReadWrite)
	FString AnswerRating = "/";

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

	UFUNCTION(BlueprintCallable)
	void GetCompletedVariantsDataSend(FString Id);
	void GetCompletedVariantsDataReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void QuestionSend(FString UserAnswer, FString CorrectAnswer);
	void QuestionGet(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void CreateVariantSend(FString Name, FString Group);
	void CreateVariantReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void GetAllQuestionsSend(FString Id);
	void GetAllQuestionsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void UpdateQuestionsSend(FString Id, TArray<FTestDataArrayStruct> Array);
	void UpdateQuestionsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


};
