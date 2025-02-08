// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "HTTPModule.h"
#include "Interfaces/IHttpResponse.h"
#include "W_PandaOSMain.generated.h"

/**
 * 
 */
UCLASS()
class MANAGER_API UW_PandaOSMain : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "User\|Authorization")
	void UserAuthorize(FString Login, FString Password);
	void UserAuthorizeAnswerReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "User\|Registration")
	void UserRegister(FString Login, FString Password, FString Name, FString Surname, FString Patronymic, int32 Group, int32 Permission);
	void UserRegisterAnswerReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UEditableTextBox> TB_Information;
};
