// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerHUD.h"
//#include "ManagerTypes.h"

void AManagerHUD::NativeConstruct()
{
	//GetGroupsSend();
}

//void AManagerHUD::SetUserData(FString m_Login, FString m_Name, FString m_Surname, FString m_Patronumic, EPermissionType m_PermissionType, FString m_GroupName)
//{
//	Login = m_Login;
//	Name = m_Name;
//	Surname = m_Surname;
//	Patronumic = m_Patronumic;
//	PermissionType = m_PermissionType;
//	GroupName = m_GroupName;
//}


FManagerUserData AManagerHUD::GetUserData_Implementation()
{
	FManagerUserData UserData;
	UserData.UserID = UserID;
	UserData.Login = Login;
	UserData.Name = Name;
	UserData.Surname = Surname;
	UserData.Patronumic = Patronumic;
	UserData.GroupName = GroupName;
	UserData.PermissionType = PermissionType;

	return UserData;
}

void AManagerHUD::SetUserData_Implementation(int32& m_UserID, FString& m_Login, FString& m_Name, FString& m_Surname, FString& m_Patronumic, EPermissionType& m_PermissionType, FString& m_GroupName)
{
	UserID = m_UserID;
	Login = m_Login;
	Name = m_Name;
	Surname = m_Surname;
	Patronumic = m_Patronumic;
	PermissionType = m_PermissionType;
	GroupName = m_GroupName;
}

void AManagerHUD::GenerateTestQuestionSend()
{
	FString URL = "http://26.76.184.253:8002/generate_testquestion";
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AManagerHUD::GenerateTestQuestionReceive);
	Request->SetTimeout(120.0f);
	Request->SetVerb("POST");
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-API-Key"), TEXT("SecretKeyFromGameEngine"));
	Request->ProcessRequest();
}

void AManagerHUD::GenerateTestQuestionReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FGeneratedTestStruct TestStruct;
	if (bWasSuccessful)
	{
		FString answer = Response->GetContentAsString();
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer);
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
		FJsonSerializer::Deserialize(JsonReader, JsonObject);

		TestStruct.QuestionType = JsonObject->GetIntegerField("question_type");
		switch (TestStruct.QuestionType)
		{
		case 0:
			TestStruct.QuestionText = JsonObject->GetStringField("question");
			TestStruct.CorrectAnswer = JsonObject->GetStringField("correct_answer");
			TestStruct.Answer_2 = JsonObject->GetStringField("answer_2");
			TestStruct.Answer_3 = JsonObject->GetStringField("answer_3");
			TestStruct.Answer_4 = JsonObject->GetStringField("answer_4");
			OnTestGenerated_Callback.Broadcast(TestStruct);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CallbackSuccess With Code 0"));
			break;
		case 1:
			TestStruct.QuestionText = JsonObject->GetStringField("question");
			OnTestGenerated_Callback.Broadcast(TestStruct);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CallbackSuccess With Code 1"));
			break;
		default:
			TestStruct.QuestionText = JsonObject->GetStringField("question");
			OnTestGenerated_Callback.Broadcast(TestStruct);
			break;
		}
	}


	
}

void AManagerHUD::AddEmployeeSend()
{
	FString URL = "http://26.76.184.253:8000/createworker";
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();




	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TArray<int> buff_ids = { 1 };
	TArray<TSharedPtr<FJsonValue>> BuffsJsonArray;

	TArray<int> debuff_ids = { 1 };
	TArray<TSharedPtr<FJsonValue>> DebuffsJsonArray;

	for (int32 buff_id : buff_ids)
	{
		BuffsJsonArray.Add(MakeShared<FJsonValueNumber>(buff_id));
	}

	for (int32 debuff_id : debuff_ids)
	{
		DebuffsJsonArray.Add(MakeShared<FJsonValueNumber>(debuff_id));
	}


	JsonObject->SetStringField(TEXT("worker_name"), TEXT("Oleg"));
	JsonObject->SetStringField(TEXT("worker_description"), TEXT("Oleg"));
	JsonObject->SetNumberField(TEXT("worker_maxhours"), 1);
	JsonObject->SetNumberField(TEXT("worker_maxsp"), 1);
	JsonObject->SetNumberField(TEXT("worker_maxtasks"), 1);
	JsonObject->SetNumberField(TEXT("worker_proficiency_3D"), 0.0f);
	JsonObject->SetNumberField(TEXT("worker_proficiency_2D"), 0.0f);
	JsonObject->SetNumberField(TEXT("worker_proficiency_Code"), 0.0f);
	JsonObject->SetArrayField(TEXT("buffs"), BuffsJsonArray);
	JsonObject->SetArrayField(TEXT("debuffs"), DebuffsJsonArray);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	// Выводим результат
	UE_LOG(LogTemp, Display, TEXT("JSON: %s"), *OutputString);


	Request->OnProcessRequestComplete().BindUObject(this, &AManagerHUD::AddEmployeeReceive);
	Request->SetTimeout(120.0f);
	Request->SetVerb("POST");
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	//Request->SetHeader(TEXT("X-API-Key"), TEXT("SecretKeyFromGameEngine"));
	Request->ProcessRequest();


}

void AManagerHUD::AddEmployeeReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnEmployeeAdded_Callback.Broadcast(true);
	}
	else
	{
		OnEmployeeAdded_Callback.Broadcast(false);
	}
}

void AManagerHUD::GetAllUsVariantsSend()
{
	FString group = GroupName;
	uint32 user_id = UserID;

	FString URL = "http://26.76.184.253:8000/getallusvariantsdata";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("group", group);
	JsonObject->SetNumberField("user_id", user_id);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetAllUsVariantsReceive);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->SetURL(URL);
	Request->ProcessRequest();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, group);
}

void AManagerHUD::GetAllUsVariantsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	TArray<FString> SimArrayIds;
	TArray<FString> CompletedSimArrayIds;

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	FString answer = Response->GetContentAsString();

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer);
	FJsonSerializer::Deserialize(JsonReader, JsonArray);

	uint32 counter = 0;


	for (const TSharedPtr<FJsonValue>& JsonValue : JsonArray)
	{
		TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();
		FString var_id = JsonObject->GetStringField("usvariant_id");
		uint32 completed = JsonObject->GetNumberField("iscompleted_by_user");


		SimArrayIds.Add(var_id);
		if (completed == 1)
		{
			CompletedSimArrayIds.Add(var_id);
		}

		GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Blue, SimArrayIds[counter]);

		counter++;
	}

	if (bWasSuccessful)
	{
		OnAllUSVariantsGot_Callback.Broadcast(true, SimArrayIds, CompletedSimArrayIds);
	}
	else
	{
		OnAllUSVariantsGot_Callback.Broadcast(false, SimArrayIds, CompletedSimArrayIds);
	}
}
