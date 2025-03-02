// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Button_Default.h"
#include "W_TextBox_Default.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "HTTPModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Manager/W_Textbox_Clickable.h"

#include "W_LoginPanel.generated.h"

/**
 * 
 */





UCLASS()
class MANAGER_API UW_LoginPanel : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:


	// WIDGET BINDING //


	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
		UW_Button_Default* BTN_LoginRegistration;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UW_Button_Default* BTN_Enter;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UW_Button_Default* BTN_Register;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* TXT_ErrorLog = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UWidgetSwitcher* WS_RegLogin;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UWidgetSwitcher* WS_PermissionGroups;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_TextBox_Default* TB_Login;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_TextBox_Default* TB_Password;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_TextBox_Default* TB_regLogin;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_TextBox_Default* TB_regPassword;

	//UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	//UW_TextBox_Default* TB_regAccessLevel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_Textbox_Clickable* TBC_regAccessLevel;

	/*UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_TextBox_Default* TB_regGroup;*/

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_Textbox_Clickable* TBC_regGroup;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_TextBox_Default* TB_regName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_TextBox_Default* TB_regSurname;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_TextBox_Default* TB_regPatronomic; //—»Õ“¿ —»◊≈— ¿ﬂ Œÿ»¡ ¿!


	//- WIDGET BINDING -//


	// VARIABLES //

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> GroupsArray;


	UPROPERTY(BlueprintReadWrite)
	TArray<FString> PermissionsArray;



	UFUNCTION(BlueprintNativeEvent)
	void OnLoginButtonClicked();
	UFUNCTION(BlueprintNativeEvent)
	void OnLoginRegistrationButtonClicked();
	UFUNCTION(BlueprintNativeEvent)
	void OnRegisterButtonClicked();
	UFUNCTION(BlueprintCallable)
	void GetGroupsSend();
	UFUNCTION(BlueprintCallable)
	void GetPermissionsSend();

	void UserAuthorizeAnswerReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void UserRegisterAnswerReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void GetGroupsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void GetPermissionsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

};
