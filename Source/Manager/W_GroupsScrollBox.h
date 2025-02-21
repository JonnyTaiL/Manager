// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_GroupsScrollBox.generated.h"



class UScrollBox;

/**
 * 
 */
UCLASS()
class MANAGER_API UW_GroupsScrollBox : public UUserWidget
{
	GENERATED_BODY()


public:

	virtual void NativeConstruct();



	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UW_GroupsScrollBox* SCROLL_Groups;
	
};
