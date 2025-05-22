// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Manager/Interfaces/IWindowsHelper.h"

#include "WindowBodyMaster.generated.h"

/**
 * 
 */
UCLASS()
class MANAGER_API UWindowBodyMaster : public UUserWidget, public IIWindowsHelper
{
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateWindow();
	virtual void UpdateWindow_Implementation() override;
};
