// Fill out your copyright notice in the Description page of Project Settings.


#include "W_LoginPanel.h"
#include "Engine.h"
#include "Json.h"
#include "Kismet/GameplayStatics.h"
#include "ManagerTypes.h"
#include "ManagerHUD.h"
//#include "W_Button_Default.h"
//#include "W_TextBox_Default.h"
//#include "Components/WidgetSwitcher.h"
//#include "Components/TextBlock.h"

void UW_LoginPanel::NativeConstruct()
{
	BTN_Enter->OnDefaultButtonClicked.AddDynamic(this, &UW_LoginPanel::OnLoginButtonClicked);
	BTN_LoginRegistration->OnDefaultButtonClicked.AddDynamic(this, &UW_LoginPanel::OnLoginRegistrationButtonClicked);
	BTN_Register->OnDefaultButtonClicked.AddDynamic(this, &UW_LoginPanel::OnRegisterButtonClicked);


	//GetGroupsSend();
	//GetPermissionsSend();

}

void UW_LoginPanel::OnLoginButtonClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("LoginButtonClicked_CPP"));

	//Логика на нажатие кнопки входа/авторизации
	if ((TB_Login->TB_TextBox->GetText().ToString() == "") || (TB_Password->TB_TextBox->GetText().ToString() == ""))
	{
		TXT_ErrorLog->SetText(FText::FromString(TEXT("Все поля должны быть заполнены")));
	}
	else {
		FString Login = TB_Login->TB_TextBox->GetText().ToString();
		FString Password = TB_Password->TB_TextBox->GetText().ToString();

		FString URL = "http://26.76.184.253:8000/checkuserdata";
		URL = URL + "?login=" + Login + "&password=" + Password;

		TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::UserAuthorizeAnswerReceive);
		Request->SetVerb("GET");
		Request->SetURL(URL);
		Request->ProcessRequest();
	}
}

void UW_LoginPanel::OnLoginRegistrationButtonClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("LoginRegistrationButtonClicked_CPP"));



	//Логика на нажатие кнопки перехода между авторизацией и регистрацией
	switch (WS_RegLogin->GetActiveWidgetIndex())
	{
	case 0:
		WS_RegLogin->SetActiveWidgetIndex(1);

		//GetGroupsSend();
		//GetPermissionsSend();

		break;
	case 1:
		WS_RegLogin->SetActiveWidgetIndex(0);
		break;
	default:
		break;
	}
}
//
void UW_LoginPanel::OnRegisterButtonClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("RegistrationButtonClicked_CPP"));

	//Логика на нажатие кнопки регистрации

	if ((TB_regLogin->TB_TextBox->GetText().ToString() == "") || (TB_regPassword->TB_TextBox->GetText().ToString() == "") || (TB_regName->TB_TextBox->GetText().ToString() == "") || (TB_regSurname->TB_TextBox->GetText().ToString() == "") || (TB_regPatronomic->TB_TextBox->GetText().ToString() == ""))
	{
		TXT_ErrorLog->SetText(FText::FromString(TEXT("Все поля должны быть заполнены")));
	}
	else {
		FString OutputString;
		FString URL = "http://26.76.184.253:8000/createuser";

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		JsonObject->SetStringField("login", TB_regLogin->TB_TextBox->GetText().ToString());
		JsonObject->SetStringField("password", TB_regPassword->TB_TextBox->GetText().ToString());
		JsonObject->SetStringField("name", TB_regName->TB_TextBox->GetText().ToString());
		JsonObject->SetStringField("surname", TB_regSurname->TB_TextBox->GetText().ToString());
		JsonObject->SetStringField("patronymic", TB_regPatronomic->TB_TextBox->GetText().ToString());
		JsonObject->SetStringField("group", TBC_regGroup->TXT_TextBlock->GetText().ToString());
		JsonObject->SetStringField("permission", TBC_regGroup->TXT_TextBlock->GetText().ToString());


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

	



}

void UW_LoginPanel::GetGroupsSend()
{
	//Отправка запроса на получение групп массивом строк

	FString URL = "http://26.76.184.253:8000/getgroupsdata";
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetGroupsReceive);
	Request->SetVerb("GET");
	Request->SetURL(URL);
	Request->ProcessRequest();
}

void UW_LoginPanel::GetPermissionsSend()
{
	//Отправка запроса на получение уровней доступа массивом строк

	FString URL = "http://26.76.184.253:8000/getpermissionsdata";
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetPermissionsReceive);
	Request->SetVerb("GET");
	Request->SetURL(URL);
	Request->ProcessRequest();
}

void UW_LoginPanel::UserAuthorizeAnswerReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString answer = Response->GetContentAsString();
	if (answer == "-1")
	{
		TXT_ErrorLog->SetText(FText::FromString(TEXT("Пользователь не найден, зарегистрируйтесь в системе")));
	}
	else if (answer == "0")
	{
		TXT_ErrorLog->SetText(FText::FromString(TEXT("Неправильный пароль!")));

		////////////
		AHUD* Hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD();
		FManagerUserData User = Cast<IIHUDRequestData>(Hud)->GetUserData_Implementation();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, User.Name);
	}
	else
	{
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //ПАРСИНГ JSON ОТВЕТА И ПОЛУЧЕНИЕ ПОЛЯ ГРУППЫ ЧЕЛОВЕКА
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
		FJsonSerializer::Deserialize(JsonReader, JsonObject);

		FString login = JsonObject->GetStringField("login"); //Присвоение переменным данных из JSON
		FString name = JsonObject->GetStringField("name");
		FString surname = JsonObject->GetStringField("surname");
		FString patronymic = JsonObject->GetStringField("patronymic");
		FString permission_string = JsonObject->GetStringField("permission_type");
		FString group = JsonObject->GetStringField("group_name");

		EPermissionType permission;
		if (permission_string == "Admin") {
			permission = EPermissionType::Admin;
		}
		else if (permission_string == "Teacher") {
			permission = EPermissionType::Teacher;
		}
		else if (permission_string == "Student") {
			permission = EPermissionType::Student;
		}
		else {
			permission = EPermissionType::Student;
		}


		AHUD* Hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD();
		if (Hud->GetClass()->ImplementsInterface(UIHUDRequestData::StaticClass()))
		{
			IIHUDRequestData::Execute_SetUserData(Hud, login, name, surname, patronymic, permission, group);
		}
		



	}
}

void UW_LoginPanel::UserRegisterAnswerReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString answer = Response->GetContentAsString();
	if (answer == "0") {
		TXT_ErrorLog->SetText(FText::FromString(TEXT("Пользователь уже есть"))); //ОШИБКА С РУССКИМ ЯЗЫКОМ
	}
	else
	{
		switch (WS_RegLogin->GetActiveWidgetIndex())
		{
		case 0:
			WS_RegLogin->SetActiveWidgetIndex(1);
			TXT_ErrorLog->SetText(FText::FromString(""));
			break;
		case 1:
			WS_RegLogin->SetActiveWidgetIndex(0);
			TXT_ErrorLog->SetText(FText::FromString(""));
			break;
		default:
			break;
		}
	}
}

void UW_LoginPanel::GetGroupsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) // Получение и обработка массива строк групп
{
	TArray<FString> StringArray;
	
	GroupsArray.Empty();

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	FString answer = Response->GetContentAsString();

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //ПАРСИНГ JSON ОТВЕТА Массива
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	FJsonSerializer::Deserialize(JsonReader, JsonArray);

	for (const TSharedPtr<FJsonValue>& JsonValue : JsonArray)
	{
		if (JsonValue->Type == EJson::String)
		{
			GroupsArray.Add(JsonValue->AsString());
		}
	}
}

void UW_LoginPanel::GetPermissionsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TArray<FString> StringArray;

	PermissionsArray.Empty();

	//GroupsArray.Empty();

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	FString answer = Response->GetContentAsString();

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //ПАРСИНГ JSON ОТВЕТА Массива
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	FJsonSerializer::Deserialize(JsonReader, JsonArray);

	for (const TSharedPtr<FJsonValue>& JsonValue : JsonArray)
	{
		if (JsonValue->Type == EJson::String)
		{
			//GroupsArray.Add(JsonValue->AsString());
			PermissionsArray.Add(JsonValue->AsString());
		}
	}
}

