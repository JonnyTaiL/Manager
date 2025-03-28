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

//TArray<FString> AManagerHUD::GetGroups()
//{
//	return GroupsArray;
//}


//void AManagerHUD::GetGroupsSend()
//{
//	//Отправка запроса на получение групп массивом строк
//
//	FString URL = "http://26.76.184.253:8000/getgroupsdata";
//	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
//	Request->OnProcessRequestComplete().BindUObject(this, AManagerHUD::GetGroupsReceive);
//	Request->SetVerb("GET");
//	Request->SetURL(URL);
//	Request->ProcessRequest();
//}
//
//void AManagerHUD::GetGroupsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) // Получение и обработка массива строк групп
//{
//	TArray<FString> StringArray;
//
//	GroupsArray.Empty();
//
//	TArray<TSharedPtr<FJsonValue>> JsonArray;
//	FString answer = Response->GetContentAsString();
//
//	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //ПАРСИНГ JSON ОТВЕТА Массива
//	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
//	FJsonSerializer::Deserialize(JsonReader, JsonArray);
//
//	for (const TSharedPtr<FJsonValue>& JsonValue : JsonArray)
//	{
//		if (JsonValue->Type == EJson::String)
//		{
//			GroupsArray.Add(JsonValue->AsString());
//		}
//	}
//}
