// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Manager/ManagerTypes.h"

#include "IHUDRequestData.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIHUDRequestData : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MANAGER_API IIHUDRequestData
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FManagerUserData GetUserData();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetUserData(FString& m_Login, FString& m_Name, FString& m_Surname, FString& m_Patronumic, EPermissionType& m_PermissionType, FString& m_GroupName);


};
