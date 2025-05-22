// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "W_OSButtonOptions.generated.h"

/**
 * 
 */

class UTextBlock;
class UW_Button_Default;

UCLASS()
class MANAGER_API UW_OSButtonOptions : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;


public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_Button_Default* BTN_Settings;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_Button_Default* BTN_Exit;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_Name;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_Group;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_AccessLevel;


	UFUNCTION(BlueprintCallable)
	void UpdateInfo();
};
