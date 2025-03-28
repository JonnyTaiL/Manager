// Fill out your copyright notice in the Description page of Project Settings.


#include "W_TestsSlot.h"

void UW_TestsSlot::NativeConstruct()
{
}

void UW_TestsSlot::DeleteVariantSend(FString m_TestId, FString m_QuestionId)
{
	FString OutputString;
	FString URL = "http://26.76.184.253:8000/deletevariantorquestion";


	//AManagerHUD* Hud = Cast<AManagerHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());



	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("variant_id", m_TestId);
	JsonObject->SetStringField("question_id", m_QuestionId);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	//	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::UserAuthorizeAnswerReceive);
	Request->SetVerb("POST");
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->ProcessRequest();
}

void UW_TestsSlot::DeleteVariantReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}

//void UW_TestsSlot::UpdateVariantSend(FString Id, FString Name, FString Group)
//{
//	FString OutputString;
//	FString URL = "http://26.76.184.253:8000/updatevariant";
//
//	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
//	JsonObject->SetStringField("variant_id", TB_Login->TB_TextBox->GetText().ToString());
//	JsonObject->SetStringField("variant_name", TB_Password->TB_TextBox->GetText().ToString());
//	JsonObject->SetStringField("group", TB_Password->TB_TextBox->GetText().ToString());
//	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
//	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
//
//	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
//	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::UserAuthorizeAnswerReceive);
//	Request->SetVerb("POST");
//	Request->SetURL(URL);
//	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
//	Request->SetContentAsString(OutputString);
//	Request->ProcessRequest();
//}
//
//void UW_TestsSlot::UpdateVariantReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
//{
// 	FString answer = Response->GetContentAsString();
//if (answer == "-1")
//{
//	TXT_ErrorLog->SetText(FText::FromString(TEXT("Пользователь не найден, зарегистрируйтесь в системе")));
//}
//else if (answer == "0")
//{
//	TXT_ErrorLog->SetText(FText::FromString(TEXT("Неправильный пароль!")));
//
//	////////////
//	AHUD* Hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD();
//	FManagerUserData User = Cast<IIHUDRequestData>(Hud)->GetUserData_Implementation();
//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, User.Name);
//}
//else
//{
//	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //ПАРСИНГ JSON ОТВЕТА И ПОЛУЧЕНИЕ ПОЛЯ ГРУППЫ ЧЕЛОВЕКА
//	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
//	FJsonSerializer::Deserialize(JsonReader, JsonObject);
//
//	int32 UserID = JsonObject->GetNumberField("user_id");
//	FString login = JsonObject->GetStringField("login"); //Присвоение переменным данных из JSON
//	FString name = JsonObject->GetStringField("name");
//	FString surname = JsonObject->GetStringField("surname");
//	FString patronymic = JsonObject->GetStringField("patronymic");
//	FString permission_string = JsonObject->GetStringField("permission_type");
//	FString group = JsonObject->GetStringField("group_name");
//
//
//
//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::FromInt(UserID));
//
//
//	EPermissionType permission;
//	if (permission_string == "Admin") {
//		permission = EPermissionType::Admin;
//	}
//	else if (permission_string == "Teacher") {
//		permission = EPermissionType::Teacher;
//	}
//	else if (permission_string == "Student") {
//		permission = EPermissionType::Student;
//	}
//	else {
//		permission = EPermissionType::Student;
//	}
//
//
//
//	AHUD* Hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD();
//	if (Hud->GetClass()->ImplementsInterface(UIHUDRequestData::StaticClass()))
//	{
//		IIHUDRequestData::Execute_SetUserData(Hud, UserID, login, name, surname, patronymic, permission, group);
//	}
//
//	this->RemoveFromParent();
//
//
//}
//}
