// Fill out your copyright notice in the Description page of Project Settings.


#include "W_PandaOSMain.h"

void UW_PandaOSMain::NativeConstruct()
{
	Super::NativeConstruct();
}

void UW_PandaOSMain::UserAuthorize(FString Login, FString Password)
{
	FString URL = "http://26.76.184.253:8000/checkuserdata";
	URL = URL + "?login=" + Login + "&password=" + Password;

	
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::UserAuthorizeAnswerReceive);
	Request->SetVerb("GET");
	Request->SetURL(URL);
	Request->ProcessRequest();
}

void UW_PandaOSMain::UserAuthorizeAnswerReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if(Response)
	{
		FString answer = Response->GetContentAsString();
	}
	
	/*if (answer == "-1")
	{
		TB_Information->SetText(FText::FromString("User is not found, try to register in our system"));
	}
	else if (answer == "0")
	{
		TB_Information->SetText(FText::FromString("Incorrect PASSWORD"));
	}
	else
	{
		if (WB_Authorized) {
			UUserWidget* WB_Auth = CreateWidget<UUserWidget>(GetWorld(), WB_Authorized);
			WB_Auth->AddToViewport();
		}
	}*/
}

void UW_PandaOSMain::UserRegister(FString Login, FString Password, FString Name, FString Surname, FString Patronymic, int32 Group, int32 Permission)
{
	FString OutputString;
	FString URL = "http://26.76.184.253:8000/createuser";

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("login", Login);
	JsonObject->SetStringField("password", Password);
	JsonObject->SetStringField("name", Name);
	JsonObject->SetStringField("surname", Surname);
	JsonObject->SetStringField("patronymic", Patronymic);
	JsonObject->SetNumberField("group", Group);
	JsonObject->SetNumberField("permission", Permission);


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

void UW_PandaOSMain::UserRegisterAnswerReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (Response)
	{
		FString answer = Response->GetContentAsString();
	}


}

