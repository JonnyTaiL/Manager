// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Manager/W_Button_Default.h"
#include "Manager/ManagerTypes.h"

#include "W_TestsSlot.generated.h"

/**
 * 
 */
UCLASS()
class MANAGER_API UW_TestsSlot : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct();


public:

	// WIDGET BINDING //


	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_Button_Default* BTN_SelectTest;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_Button_Default* BTN_EditTest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	EPermissionType AccessLevel;


	
};
