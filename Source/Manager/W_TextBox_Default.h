// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"



#include "W_TextBox_Default.generated.h"

/**
 * 
 */
UCLASS()
class MANAGER_API UW_TextBox_Default : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UEditableTextBox* TB_TextBox = nullptr;


};
