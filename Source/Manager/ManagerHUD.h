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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAllUSVariantsGot, bool, Success, const TArray<FString>&, SimArrayIds, const TArray<FString>&, CompletedSimArrayIds);// Код Максима





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


	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnTestGenerated OnTestGenerated_Callback;
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnEmployeeAdded OnEmployeeAdded_Callback;
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")// Код Максима
	FOnAllUSVariantsGot OnAllUSVariantsGot_Callback;// Код Максима

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FManagerUserData GetUserData();
	virtual FManagerUserData GetUserData_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetUserData(int32& m_UserID, FString& m_Login, FString& m_Name, FString& m_Surname, FString& m_Patronumic, EPermissionType& m_PermissionType, FString& m_GroupName);
	virtual void SetUserData_Implementation(int32& m_UserID, FString& m_Login, FString& m_Name, FString& m_Surname, FString& m_Patronumic, EPermissionType& m_PermissionType, FString& m_GroupName) override;
	

	UFUNCTION(BlueprintCallable)
	void GenerateTestQuestionSend();
	void GenerateTestQuestionReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void AddEmployeeSend();
	void AddEmployeeReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable) // Код Максима
	void GetAllUsVariantsSend();
	void GetAllUsVariantsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);// Код Максима
};
