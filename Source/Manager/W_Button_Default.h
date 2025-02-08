// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "W_Button_Default.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDefaultButtonClickedEvent);

UCLASS()
class MANAGER_API UW_Button_Default : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;



public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TXT_ButtonText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton* BTN_Button = nullptr;



	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDefaultButtonClickedEvent OnDefaultButtonClicked;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Event_OnButtonClicked();
		void Event_OnButtonClicked_Implementation();

	
};
