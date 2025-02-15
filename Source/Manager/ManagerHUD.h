// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interfaces/IHUDRequestData.h"
//#include "ManagerTypes.h"
#include "ManagerHUD.generated.h"



/**
 * 
 */
UCLASS()
class MANAGER_API AManagerHUD : public AHUD, public IIHUDRequestData
{
	GENERATED_BODY()

private:

	virtual void NativeConstruct();



protected:

	UFUNCTION()
	void SetUserData(FString m_Login, FString m_Name, FString m_Surname, FString m_Patronumic, EPermissionType m_PermissionType, FString m_GroupName);

public:
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





	FManagerUserData GetUserData_Implementation() override;
	
};
