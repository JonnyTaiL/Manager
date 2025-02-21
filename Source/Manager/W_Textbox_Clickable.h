// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

#include "W_Textbox_Clickable.generated.h"

/**
 * 
 */
//class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTextboxButtonClickedEvent);

UCLASS()
class MANAGER_API UW_Textbox_Clickable : public UUserWidget
{
	GENERATED_BODY()
	

	virtual void NativeConstruct() override;

public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_TextBlock = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* BTN_Button = nullptr;


	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTextboxButtonClickedEvent OnDefaultButtonClicked;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Event_OnButtonClicked();
	void Event_OnButtonClicked_Implementation();

};
