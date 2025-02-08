// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Button_Default.h"
#include "W_TextBox_Default.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"


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


	UFUNCTION(BlueprintNativeEvent)
	void OnEnterButtonClicked();
	UFUNCTION(BlueprintNativeEvent)
	void OnLoginRegistrationButtonClicked();
	UFUNCTION(BlueprintNativeEvent)
	void OnRegisterButtonClicked();

};
