// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Manager/ManagerTypes.h"

#include "W_SimSlot.generated.h"

/**
 * 
 */
UCLASS()
class MANAGER_API UW_SimSlot : public UUserWidget
{
	GENERATED_BODY()
	

	virtual void NativeConstruct();

public:

	// WidgetBinding //
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UW_Button_Default* BTN_SelectTest;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UW_Button_Default* BTN_EditTest;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UW_Button_Default* BTN_DeleteTest;


	// Variables //

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPermissionType AccessLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FString TestId;

};
