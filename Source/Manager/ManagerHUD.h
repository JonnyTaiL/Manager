// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interfaces/IHUDRequestData.h"
#include "Interfaces/IHttpResponse.h"
#include "ManagerTypes.h"
#include "HTTPModule.h"
#include "ManagerHUD.generated.h"



/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestGenerated, const FGeneratedTestStruct&, TestStruct);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEmployeeAdded, bool, Success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSimVariantIdsReceived, bool, Success, const TArray<FString>&, SimArrayIds, const TArray<FString>&, CompletedSimArrayIds);// Êîä Ìàêñèìà

// 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVariantsIdsReceived, bool, Success, const TArray<FString>&, TestArrayIds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompletedTestVariantAdded, bool, Success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCompletedTestVariantsIdsReceived, bool, Success, const TArray<FString>&, CompletedSimArrayIds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestionRated, bool, Success, const FString&, Answer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestVariantCreated, bool, Success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAllQuestionsReceived, bool, Success, const TArray<FTestDataArrayStruct>&, Questions);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestionUpdated, bool, Success);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSimVariantCreated, bool, Success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSimVariantDeleted, bool, Success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnModifiersDataReceived, bool, Success, const TArray<FModifierData>&, buffs, const TArray<FModifierData>&, debuffs);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSimVariantDataReceived, bool, Success, const FSimVariantData&, SimVariantData);

// 

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestDataArrayReceived, bool, Success);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompletedVariantsReceived, bool, Success);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestionReceiced, bool, Success);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllQuestionsReveived, bool, Success);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestionsUpdated, bool, Success);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVariantCreated, bool, Success);



UCLASS()
class MANAGER_API AManagerHUD : public AHUD, public IIHUDRequestData
{
	GENERATED_BODY()

private:

	virtual void NativeConstruct();



protected:


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UserID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Login;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Surname;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Patronumic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPermissionType PermissionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GroupName;




	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> GroupsArray;
	UPROPERTY(BlueprintReadWrite)
	FSimVariantData SimVariantData;




	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FManagerUserData GetUserData();
	virtual FManagerUserData GetUserData_Implementation() override;


	//
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnVariantsIdsReceived OnVariantsIdsReceived_Callback;
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnCompletedTestVariantAdded OnCompletedTestVariantAdded_Callback;
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnCompletedTestVariantsIdsReceived OnCompletedVariantsIdsReceived_Callback;
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnQuestionRated OnQuestionRated_Callback;
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnTestVariantCreated OnTestVariantCreated_Callback;
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnAllQuestionsReceived OnAllQuestionsReceived_Callback;
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnQuestionUpdated OnQuestionUpdated_Callback;



	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnTestGenerated OnTestGenerated_Callback;
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnEmployeeAdded OnEmployeeAdded_Callback;
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnSimVariantIdsReceived OnSimVariantIdsReceived_Callback;
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnSimVariantDataReceived OnSimVariantDataReceived_Callback;

	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnSimVariantCreated OnSimVariantCreated_Callback;
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnSimVariantDeleted OnSimVariantDeleted_Callback;
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnModifiersDataReceived OnModifiersDataReceived_Callback;

	//



	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetUserData(int32& m_UserID, FString& m_Login, FString& m_Name, FString& m_Surname, FString& m_Patronumic, EPermissionType& m_PermissionType, FString& m_GroupName);
	virtual void SetUserData_Implementation(int32& m_UserID, FString& m_Login, FString& m_Name, FString& m_Surname, FString& m_Patronumic, EPermissionType& m_PermissionType, FString& m_GroupName) override;
	

	UFUNCTION(BlueprintCallable)
	void GenerateTestQuestionSend();
	void GenerateTestQuestionReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void AddEmployeeSend(FEmployeeData EmployeeData);
	void AddEmployeeReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	// TESTS //

	UFUNCTION(BlueprintCallable)
	void GetTestVariantsDataSend();
	void GetTestVariantsDataReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void AddCompletedTestVariantSend(int32 m_UserId, int32 m_VarID, int32 m_Score);
	void AddCompleteTestVariantRecieve(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void GetCompletedTestVariantsIdsSend(FString Id);
	void GetCompletedTestVariantsIdsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void RateCustomAnswerSend(FString UserAnswer, FString CorrectAnswer);
	void RateCustomAnswerReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void CreateTestVariantSend(FString m_Name, FString m_Group);
	void CreateTestVariantReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void GetAllQuestionsSend(FString Id);
	void GetAllQuestionsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void UpdateQuestionsSend(FString Id, TArray<FTestDataArrayStruct> Array);
	void UpdateQuestionsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);



	// SIMULATION //

	UFUNCTION(BlueprintCallable)
	void GetSimVariantsIdsSend();
	void GetSimVariantsIdsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void GetSimVariantDataSend(int32 m_Var_ID);
	void GetSimVariantDataReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void CreateSimVariantSend(FString m_VariantName);
	void CreateSimVariantRecive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void DeleteVariantSend(int32 m_Var_ID);
	void DeleteVariantRecive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void GetModifiersSend();
	void GetModifiersReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);



};
