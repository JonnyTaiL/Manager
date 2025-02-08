// Fill out your copyright notice in the Description page of Project Settings.


#include "W_LoginPanel.h"
#include "Engine.h"

void UW_LoginPanel::NativeConstruct()
{
	BTN_Enter->OnDefaultButtonClicked.AddDynamic(this, &UW_LoginPanel::OnLoginButtonClicked);
	BTN_LoginRegistration->OnDefaultButtonClicked.AddDynamic(this, &UW_LoginPanel::OnLoginRegistrationButtonClicked);
	BTN_Register->OnDefaultButtonClicked.AddDynamic(this, &UW_LoginPanel::OnRegisterButtonClicked);
}

void UW_LoginPanel::OnLoginButtonClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("LoginButtonClicked_CPP"));

	//Логика на нажатие кнопки входа/авторизации

	FString Login = TB_Login->TB_TextBox->GetText().ToString();
	FString Password = TB_Password->TB_TextBox->GetText().ToString();

	FString URL = "http://127.0.0.1:8000/checkuserdata";
	URL = URL + "?login=" + Login + "&password=" + Password;

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::UserAuthorizeAnswerReceive);
	Request->SetVerb("GET");
	Request->SetURL(URL);
	Request->ProcessRequest();


}

void UW_LoginPanel::OnLoginRegistrationButtonClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("LoginRegistrationButtonClicked_CPP"));

	//Логика на нажатие кнопки перехода между авторизацией и регистрацией
	switch (WS_RegLogin->GetActiveWidgetIndex())
	{
	case 0:
		WS_RegLogin->SetActiveWidgetIndex(1);
		break;
	case 1:
		WS_RegLogin->SetActiveWidgetIndex(0);
		break;
	default:
		break;
	}
}

void UW_LoginPanel::OnRegisterButtonClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("RegistrationButtonClicked_CPP"));

	//Логика на нажатие кнопки регистрации

	FString OutputString;
	FString URL = "http://127.0.0.1:8000/createuser";
	FString Group = TB_regGroup->TB_TextBox->GetText().ToString();
	FString Permission = TB_regGroup->TB_TextBox->GetText().ToString();

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("login", TB_regLogin->TB_TextBox->GetText().ToString());
	JsonObject->SetStringField("password", TB_regPassword->TB_TextBox->GetText().ToString());
	JsonObject->SetStringField("name", TB_regName->TB_TextBox->GetText().ToString());
	JsonObject->SetStringField("surname", TB_regSurname->TB_TextBox->GetText().ToString());
	JsonObject->SetStringField("patronymic", TB_regPatronomic->TB_TextBox->GetText().ToString());
	JsonObject->SetNumberField("group", FCString::Atoi(*Group));
	JsonObject->SetNumberField("permission", FCString::Atoi(*Permission));


	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::UserRegisterAnswerReceive);
	Request->SetVerb("POST");
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->ProcessRequest();




}

void UW_LoginPanel::UserAuthorizeAnswerReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString answer = Response->GetContentAsString();
	if (answer == "-1")
	{
		TXT_ErrorLog->SetText(FText::FromString("User is not found, try to register in our system"));
	}
	else if (answer == "0")
	{
		TXT_ErrorLog->SetText(FText::FromString("Incorrect PASSWORD"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("User is authorized!!!"));
	}
}

void UW_LoginPanel::UserRegisterAnswerReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString answer = Response->GetContentAsString();
	switch (WS_RegLogin->GetActiveWidgetIndex())
	{
	case 0:
		WS_RegLogin->SetActiveWidgetIndex(1);
		break;
	case 1:
		WS_RegLogin->SetActiveWidgetIndex(0);
		break;
	default:
		break;
	}
	
}

