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
	//UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	//UScrollBox* SCROLL_Groups;


	//  Change To Scrollbox !!!!!!!!
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_TextBox_Default* TB_Group;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_TextBox_Default* TB_VarName;
	//UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	//UW_TextBox_Default* TB_CustomAnswer;




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

	//UPROPERTY(BlueprintReadWrite)
	//FString Depr_CorrectAnswer;
	//UPROPERTY(BlueprintReadWrite)
	//FString Depr_UserAnswer;
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

	//UFUNCTION(BlueprintCallable)
	//void QuestionSend(FString UserAnswer, FString CorrectAnswer);
	//void QuestionGet(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void CreateVariantSend(FString Name, FString Group);
	//void CreateVariantReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);



	UFUNCTION(BlueprintCallable)
	void GetAllQuestionsSend(FString Id);
	void GetAllQuestionsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void UpdateQuestionsSend(FString Id);
	void UpdateQuestionsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	//


};
