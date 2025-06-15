// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerHUD.h"
#include "ManagerConfig.h"
//#include "ManagerTypes.h"









void AManagerHUD::NativeConstruct()
{
	//GetGroupsSend();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, "http://" + Config::SERVER_IP + "/createuser");
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

void AManagerHUD::GetAllEmployeesSend()
{
	FString URL = "http://" + Config::SERVER_IP + "/getallworkers";

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetCompletedTestVariantsIdsReceive);
	Request->SetVerb("GET");
	Request->SetURL(URL);
	Request->ProcessRequest();
}






void AManagerHUD::GetAllEmployeesReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TArray<FEmployeeData> Employees;

	//	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
		FString answer = Response->GetContentAsString();


		TArray<TSharedPtr<FJsonValue>> JsonArray;
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); 

		FJsonSerializer::Deserialize(JsonReader, JsonArray);


//		const TArray<TSharedPtr<FJsonValue>>* EmployeesArray;
	//	if (JsonObject->TryGetArrayField("workers", EmployeesArray))
	//	{
			for (const TSharedPtr<FJsonValue>& JsonElement : JsonArray)
			{
				const TSharedPtr<FJsonObject> EmployeeObj = JsonElement->AsObject();
				if (EmployeeObj.IsValid())
				{
					FEmployeeData Employee;


					int32 EmployeeID = EmployeeObj->GetIntegerField("worker_id");
					FString Employee_Name = EmployeeObj->GetStringField("worker_name");
					FString Employee_Description = EmployeeObj->GetStringField("worker_description");
					int32 Employee_MaxHours = EmployeeObj->GetIntegerField("worker_maxhours");
					int32 Employee_MaxTasks = EmployeeObj->GetIntegerField("worker_maxtasks");
					int32 Employee_Skill = EmployeeObj->GetIntegerField("worker_maxsp");
					float Employee_Prof_3D = EmployeeObj->GetNumberField("worker_3D");
					float Employee_Prof_2D = EmployeeObj->GetNumberField("worker_2D");
					float Employee_Prof_Code = EmployeeObj->GetNumberField("worker_code");



					// Get Modifiers

					//// Buffs
					//const TArray<TSharedPtr<FJsonValue>>* BuffsArray;
					//if (EmployeeObj->TryGetArrayField("buffs", BuffsArray))
					//{
					//	for (const TSharedPtr<FJsonValue>& ModifierValue : *BuffsArray)
					//	{
					//		const TSharedPtr<FJsonObject> ModifierObj = ModifierValue->AsObject();
					//		if (ModifierObj.IsValid())
					//		{
					//			FModifierData Buff;
					//			Buff.ID = ModifierObj->GetIntegerField("buff_id");
					//			Buff.Name = ModifierObj->GetStringField("buff_name");
					//			Buff.Description = ModifierObj->GetStringField("buff_description");
					//			Buff.USCompleteChance = ModifierObj->GetNumberField("buff_USCompleteChance");
					//			Buff.MaxSPModificator = ModifierObj->GetNumberField("buff_MaxSPModificator");
					//			Buff.MaxHoursModificator = ModifierObj->GetNumberField("buff_MaxHoursModificator");
					//			Buff.MaxTasksModificator = ModifierObj->GetNumberField("buff_MaxTasksModificator");

					//			// Add to struct here
					//			Employee.Buffs.Add(Buff);

					//		}
					//	}
					//}


					//// Debuffs
					//const TArray<TSharedPtr<FJsonValue>>* DebuffsArray;
					//if (EmployeeObj->TryGetArrayField("debuffs", DebuffsArray))
					//{
					//	for (const TSharedPtr<FJsonValue>& ModifierValue : *DebuffsArray)
					//	{
					//		const TSharedPtr<FJsonObject> ModifierObj = ModifierValue->AsObject();
					//		if (ModifierObj.IsValid())
					//		{
					//			FModifierData Debuff;
					//			Debuff.ID = ModifierObj->GetIntegerField("debuff_id");
					//			Debuff.Name = ModifierObj->GetStringField("debuff_name");
					//			Debuff.Description = ModifierObj->GetStringField("debuff_description");
					//			Debuff.USCompleteChance = ModifierObj->GetNumberField("debuff_USCompleteChance");
					//			Debuff.MaxSPModificator = ModifierObj->GetNumberField("debuff_MaxSPModificator");
					//			Debuff.MaxHoursModificator = ModifierObj->GetNumberField("debuff_MaxHoursModificator");
					//			Debuff.MaxTasksModificator = ModifierObj->GetNumberField("debuff_MaxTasksModificator");

					//			// Add to struct here
					//			Employee.Debuffs.Add(Debuff);
					//		}
					//	}
					//}

					Employee.ID = EmployeeID;
					Employee.Name = Employee_Name;
					Employee.Description = Employee_Description;
					Employee.MaxHours = Employee_MaxHours;
					Employee.MaxTasks = Employee_MaxTasks;
					Employee.Skill = Employee_Skill;
					Employee.Proficiency.Modeling = Employee_Prof_3D;
					Employee.Proficiency.Art = Employee_Prof_2D;
					Employee.Proficiency.Code = Employee_Prof_Code;

					// Add Employee to sim data array
					Employees.Add(Employee);

				}
			}

			OnAllEmployeesReceived_Callback.Broadcast(true, Employees);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, Employees[0].Name);
	//	}

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, "NOT SUCCESSFUL");
	}
}

void AManagerHUD::AddEmployeeSend(FEmployeeData EmployeeData)
{
	FString URL = "http://" + Config::SERVER_IP + "/createworker";
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();


	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	TArray<FModifierData> Buffs = EmployeeData.Buffs;
	TArray<FModifierData> Debuffs = EmployeeData.Debuffs;

	TArray<TSharedPtr<FJsonValue>> BuffsJsonArray;
	TArray<TSharedPtr<FJsonValue>> DebuffsJsonArray;

	for (FModifierData Buff : Buffs)
	{

		BuffsJsonArray.Add(MakeShared<FJsonValueNumber>(Buff.ID));
	}

	for (FModifierData Debuff : Debuffs)
	{
		DebuffsJsonArray.Add(MakeShared<FJsonValueNumber>(Debuff.ID));
	}

	

	JsonObject->SetStringField(TEXT("worker_name"), EmployeeData.Name);
	JsonObject->SetStringField(TEXT("worker_description"), EmployeeData.Description);
	JsonObject->SetNumberField(TEXT("worker_maxhours"), EmployeeData.MaxHours);
	JsonObject->SetNumberField(TEXT("worker_maxsp"), EmployeeData.Skill);
	JsonObject->SetNumberField(TEXT("worker_maxtasks"), EmployeeData.MaxTasks);
	JsonObject->SetNumberField(TEXT("worker_proficiency_3D"), EmployeeData.Proficiency.Modeling);
	JsonObject->SetNumberField(TEXT("worker_proficiency_2D"), EmployeeData.Proficiency.Art);
	JsonObject->SetNumberField(TEXT("worker_proficiency_Code"), EmployeeData.Proficiency.Code);
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
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("--Employee Added--"));
	}
	else
	{
		OnEmployeeAdded_Callback.Broadcast(false);
	}
}



void AManagerHUD::DeleteEmployeeSend(int32 m_ID)
{
	FString URL = "http://" + Config::SERVER_IP + "/deleteworker";
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());


	JsonObject->SetNumberField(TEXT("worker_id"), m_ID);


	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	// Выводим результат
	UE_LOG(LogTemp, Display, TEXT("JSON: %s"), *OutputString);


	Request->OnProcessRequestComplete().BindUObject(this, &AManagerHUD::DeleteEmployeeReceive);
	Request->SetTimeout(120.0f);
	Request->SetVerb("POST");
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	//Request->SetHeader(TEXT("X-API-Key"), TEXT("SecretKeyFromGameEngine"));
	Request->ProcessRequest();
}

void AManagerHUD::DeleteEmployeeReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnEmployeeDeleted_Callback.Broadcast(true);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("--Employee Deleted--"));
	}
}

void AManagerHUD::UpdateEmployeeSend(FEmployeeData EmployeeData)
{
	FString URL = "http://" + Config::SERVER_IP + "/updateworker";
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();


	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	//TArray<int> buff_ids = { 1 };
	TArray<FModifierData> Buffs = EmployeeData.Buffs;
	TArray<FModifierData> Debuffs = EmployeeData.Debuffs;
	TArray<TSharedPtr<FJsonValue>> BuffsJsonArray;

	TArray<int> debuff_ids = { 1 };
	TArray<TSharedPtr<FJsonValue>> DebuffsJsonArray;

	for (FModifierData Buff : Buffs)
	{

		BuffsJsonArray.Add(MakeShared<FJsonValueNumber>(Buff.ID));
	}

	for (FModifierData Debuff : Debuffs)
	{
		DebuffsJsonArray.Add(MakeShared<FJsonValueNumber>(Debuff.ID));
	}


	JsonObject->SetNumberField(TEXT("worker_id"), EmployeeData.ID);
	JsonObject->SetStringField(TEXT("worker_name"), EmployeeData.Name);
	JsonObject->SetStringField(TEXT("worker_description"), EmployeeData.Description);
	JsonObject->SetNumberField(TEXT("worker_maxhours"), EmployeeData.MaxHours);
	JsonObject->SetNumberField(TEXT("worker_maxsp"), EmployeeData.Skill);
	JsonObject->SetNumberField(TEXT("worker_maxtasks"), EmployeeData.MaxTasks);
	JsonObject->SetNumberField(TEXT("worker_proficiency_3D"), EmployeeData.Proficiency.Modeling);
	JsonObject->SetNumberField(TEXT("worker_proficiency_2D"), EmployeeData.Proficiency.Art);
	JsonObject->SetNumberField(TEXT("worker_proficiency_Code"), EmployeeData.Proficiency.Code);
	JsonObject->SetArrayField(TEXT("buffs"), BuffsJsonArray);
	JsonObject->SetArrayField(TEXT("debuffs"), DebuffsJsonArray);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);


	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::UpdateEmployeeReceive);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->SetURL(URL);
	Request->ProcessRequest();
}

void AManagerHUD::UpdateEmployeeReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnEmployeeUpdated_Callback.Broadcast(true);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("--Employee Updated--"));
	}
}

void AManagerHUD::GetAllUserStoriesSend()
{
	FString URL = "http://" + Config::SERVER_IP + "/getalluserstories";

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetAllUserStoriesReceive);
	Request->SetVerb("GET");
	Request->SetURL(URL);
	Request->ProcessRequest();

}

void AManagerHUD::GetAllUserStoriesReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	FString answer = Response->GetContentAsString();
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer);

	const TArray<TSharedPtr<FJsonValue>>* UserStoriesArray;
	TArray<FUSData> UserStories;

	if (JsonObject->TryGetArrayField("UserStories", UserStoriesArray))
	{
		for (const TSharedPtr<FJsonValue>& StoryValue : *UserStoriesArray)
		{
			const TSharedPtr<FJsonObject> StoryObj = StoryValue->AsObject();
			if (StoryObj.IsValid())
			{
				FUSData USData;

				int32 US_ID = StoryObj->GetIntegerField("us_id");
				FString US_Description = StoryObj->GetStringField("us_description");
				int32 US_Complexity = StoryObj->GetIntegerField("us_compexity");
				int32 US_Hours = StoryObj->GetIntegerField("us_hours");


				//  Get DoBefore Array
				const TArray<TSharedPtr<FJsonValue>>* DoBeforeArray;
				if (StoryObj->TryGetArrayField("us_dobefore", DoBeforeArray))
				{
					for (const TSharedPtr<FJsonValue>& Value : *DoBeforeArray)
					{
						int32 ID = Value->AsNumber();
						USData.DoBefore.Add(ID);
					}
				}
				//  Get Children Array
				const TArray<TSharedPtr<FJsonValue>>* DoughterArray;
				if (StoryObj->TryGetArrayField("us_doughter", DoughterArray))
				{
					for (const TSharedPtr<FJsonValue>& Value : *DoughterArray)
					{
						USData.ChildUS.Add(Value->AsNumber());
					}
				}
				//  Get Parents Array
				const TArray<TSharedPtr<FJsonValue>>* ParentArray;
				if (StoryObj->TryGetArrayField("us_parent", ParentArray))
				{
					for (const TSharedPtr<FJsonValue>& Value : *ParentArray)
					{
						USData.ParentUS.Add(Value->AsNumber());
					}
				}



				bool US_3D = StoryObj->GetBoolField("3D");
				bool US_2D = StoryObj->GetBoolField("2D");
				bool US_Code = StoryObj->GetBoolField("Code");

				USData.ID = US_ID;
				USData.Description = US_Description;
				USData.Complexity = US_Complexity;
				USData.Hours = US_Hours;
				USData.Proficiencies.Modeling = US_3D;
				USData.Proficiencies.Art = US_2D;
				USData.Proficiencies.Code = US_Code;

				// Add userstories array to variant data
				UserStories.Add(USData);


				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%s"), *SimVariantData.Name));
			}
		}

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, UserStories[0].Description);
		OnAllUserStoriesReceived_Callback.Broadcast(true, UserStories);
	}
}

void AManagerHUD::AddUserStorySend(FUSData USData, FProficiencyRequare Requare)
{
	FString URL = "http://" + Config::SERVER_IP + "/createuserstory";
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	TArray<int32> DoBeforeArray = USData.DoBefore;
	TArray<int32> DoughterArray = USData.ChildUS;
	TArray<int32> ParentsArray = USData.ParentUS;

	TArray<TSharedPtr<FJsonValue>> DoBeforeJsonArray;
	TArray<TSharedPtr<FJsonValue>> DoughterJsonArray;
	TArray<TSharedPtr<FJsonValue>> ParentsJsonArray;


	for (int32 DoBeforeItem : DoBeforeArray)
	{

		DoBeforeJsonArray.Add(MakeShared<FJsonValueNumber>(DoBeforeItem));
	}

	for (int32 DoughterItem : DoughterArray)
	{
		DoughterJsonArray.Add(MakeShared<FJsonValueNumber>(DoughterItem));
	}

	for (int32 ParentsItem : ParentsArray)
	{
		ParentsJsonArray.Add(MakeShared<FJsonValueNumber>(ParentsItem));
	}

	JsonObject->SetStringField(TEXT("us_description"), USData.Description);
	JsonObject->SetNumberField(TEXT("us_complexity"), USData.Complexity);
	JsonObject->SetNumberField(TEXT("us_hours"), USData.Hours);
	JsonObject->SetArrayField(TEXT("us_dobefore"), DoBeforeJsonArray);
	JsonObject->SetArrayField(TEXT("us_doughter"), DoughterJsonArray);
	JsonObject->SetArrayField(TEXT("us_parent"), ParentsJsonArray);
	JsonObject->SetBoolField(TEXT("Modeling"), Requare.Modeling);
	JsonObject->SetBoolField(TEXT("Art"), Requare.Art);
	JsonObject->SetBoolField(TEXT("Code"), Requare.Code);




	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	// Выводим результат
	UE_LOG(LogTemp, Display, TEXT("JSON: %s"), *OutputString);


	Request->OnProcessRequestComplete().BindUObject(this, &AManagerHUD::AddUserStoryReceive);
	Request->SetTimeout(120.0f);
	Request->SetVerb("POST");
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	//Request->SetHeader(TEXT("X-API-Key"), TEXT("SecretKeyFromGameEngine"));
	Request->ProcessRequest();

}

void AManagerHUD::AddUserStoryReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnUserStoryAdded_Callback.Broadcast(true);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("--UserStory Added--"));
	}
}

void AManagerHUD::DeleteUserStorySend(int32 m_ID)
{
	FString URL = "http://" + Config::SERVER_IP + "/deleteuserstory";
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());


	JsonObject->SetNumberField(TEXT("us_id"), m_ID);


	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	// Выводим результат
	UE_LOG(LogTemp, Display, TEXT("JSON: %s"), *OutputString);


	Request->OnProcessRequestComplete().BindUObject(this, &AManagerHUD::DeleteUserStoryReceive);
	Request->SetTimeout(120.0f);
	Request->SetVerb("POST");
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	//Request->SetHeader(TEXT("X-API-Key"), TEXT("SecretKeyFromGameEngine"));
	Request->ProcessRequest();
}

void AManagerHUD::DeleteUserStoryReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnUserStoryDeleted_Callback.Broadcast(true);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("--UserStory Deleted--"));
	}
}

void AManagerHUD::UpdateUserStorySend(FUSData USData, FProficiencyRequare Requare)
{
	FString URL = "http://" + Config::SERVER_IP + "/updateuserstory";
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	TArray<int32> DoBeforeArray = USData.DoBefore;
	TArray<int32> DoAfterArray = USData.ChildUS;
	TArray<int32> ParentsArray = USData.ParentUS;

	TArray<TSharedPtr<FJsonValue>> DoBeforeJsonArray;
	TArray<TSharedPtr<FJsonValue>> DoAfterJsonArray;
	TArray<TSharedPtr<FJsonValue>> ParentsJsonArray;


	for (int32 DoBeforeItem : DoBeforeArray)
	{

		DoBeforeJsonArray.Add(MakeShared<FJsonValueNumber>(DoBeforeItem));
	}

	for (int32 DoAfterItem : DoAfterArray)
	{
		DoAfterJsonArray.Add(MakeShared<FJsonValueNumber>(DoAfterItem));
	}

	for (int32 ParentsItem : ParentsArray)
	{
		ParentsJsonArray.Add(MakeShared<FJsonValueNumber>(ParentsItem));
	}

	JsonObject->SetNumberField(TEXT("us_id"), USData.ID);
	JsonObject->SetStringField(TEXT("us_description"), USData.Description);
	JsonObject->SetNumberField(TEXT("us_complexity"), USData.Complexity);
	JsonObject->SetNumberField(TEXT("us_hours"), USData.Hours);
	JsonObject->SetArrayField(TEXT("us_dobefore"), DoBeforeJsonArray);
	JsonObject->SetArrayField(TEXT("us_doughter"), DoAfterJsonArray);
	JsonObject->SetArrayField(TEXT("us_parent"), ParentsJsonArray);
	JsonObject->SetBoolField(TEXT("Modeling"), Requare.Modeling);
	JsonObject->SetBoolField(TEXT("Art"), Requare.Art);
	JsonObject->SetBoolField(TEXT("Code"), Requare.Code);




	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	// Выводим результат
	UE_LOG(LogTemp, Display, TEXT("JSON: %s"), *OutputString);


	Request->OnProcessRequestComplete().BindUObject(this, &AManagerHUD::UpdateUserStoryReceive);
	Request->SetTimeout(120.0f);
	Request->SetVerb("POST");
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	//Request->SetHeader(TEXT("X-API-Key"), TEXT("SecretKeyFromGameEngine"));
	Request->ProcessRequest();
}

void AManagerHUD::UpdateUserStoryReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnUserStoryUpdated_Callback.Broadcast(true);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("--UserStory Updated--"));
	}
}





void AManagerHUD::GetTestVariantsDataSend()
{

	FString group = GroupName;





	FString URL = "http://" + Config::SERVER_IP + "/getvariantsdata";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("group", group);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetTestVariantsDataReceive);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetURL(URL);
	Request->SetContentAsString(OutputString);
	Request->ProcessRequest();

	
}



void AManagerHUD::GetTestVariantsDataReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TArray<FString> TestsArrayIds;
	

	if (bWasSuccessful)
	{
		TArray<TSharedPtr<FJsonValue>> JsonArray;
		FString answer = Response->GetContentAsString();

		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer);
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
		FJsonSerializer::Deserialize(JsonReader, JsonArray);

		for (const TSharedPtr<FJsonValue>& JsonValue : JsonArray)
		{

			TestsArrayIds.Add(JsonValue->AsString());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, answer);

		}

		OnVariantsIdsReceived_Callback.Broadcast(true, TestsArrayIds);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("VariantReceived"));
	}
	else
	{
		OnVariantsIdsReceived_Callback.Broadcast(false, TestsArrayIds);
	}


}

void AManagerHUD::AddCompletedTestVariantSend(int32 m_UserId, int32 m_VarID, int32 m_Score)
{
	FString URL = "http://" + Config::SERVER_IP + "/addcompletedvariants";
	FString OutputString;



	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetNumberField("user_id", m_UserId);
	JsonObject->SetNumberField("variant_id", m_VarID);
	JsonObject->SetNumberField("score", m_Score);



	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);



	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::AddCompleteTestVariantRecieve);
	Request->SetVerb("POST");
	Request->SetURL(URL);

	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->ProcessRequest();

}

void AManagerHUD::AddCompleteTestVariantRecieve(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnCompletedTestVariantAdded_Callback.Broadcast(true);
	}
}

void AManagerHUD::GetCompletedTestVariantsIdsSend(FString Id)
{
	FString URL = "http://" + Config::SERVER_IP + "/getcompletedvariantsdata";
	FString variant_id = Id;
	URL = URL + "?user_id=" + variant_id;



	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetCompletedTestVariantsIdsReceive);
	Request->SetVerb("GET");
	Request->SetURL(URL);
	Request->ProcessRequest();
}

void AManagerHUD::GetCompletedTestVariantsIdsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TArray<FString> CompletedTestsArrayIds;
	//CompletedTestsArrayIds.Empty();

	if (bWasSuccessful)
	{
		TArray<TSharedPtr<FJsonValue>> JsonArray;
		FString answer = Response->GetContentAsString();

		if (!answer.IsEmpty())
		{
			TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer);
			TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
			FJsonSerializer::Deserialize(JsonReader, JsonArray);

			for (const TSharedPtr<FJsonValue>& JsonValue : JsonArray)
			{

				CompletedTestsArrayIds.Add(JsonValue->AsString());
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, JsonValue->AsString());
			}
			//			FOnCompletedVariantsReceived_Callback.Broadcast(true);
		}

		OnCompletedVariantsIdsReceived_Callback.Broadcast(true, CompletedTestsArrayIds);
	}
	else
	{
		//		FOnCompletedVariantsReceived_Callback.Broadcast(false);
	}
}

void AManagerHUD::RateCustomAnswerSend(FString UserAnswer, FString CorrectAnswer)
{
	FString OutputString;

	FString URL = "http://26.76.184.253:8001/compare_answers";
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("UserAnswer", UserAnswer);
	JsonObject->SetStringField("CorrectAnswer", CorrectAnswer);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::RateCustomAnswerReceive);
	Request->SetVerb("POST");
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->ProcessRequest();
}

void AManagerHUD::RateCustomAnswerReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString AnswerRating = "/";

	if (bWasSuccessful)
	{
		FString answer = Response->GetContentAsString();
		AnswerRating = answer;
		OnQuestionRated_Callback.Broadcast(true, AnswerRating);
	}
	else
	{
		//		FOnQuestionReceiced_Callback.Broadcast(false);
	}
}

void AManagerHUD::CreateTestVariantSend(FString m_Name, FString m_Group)
{
	FString OutputString;
	FString URL = "http://" + Config::SERVER_IP + "/createvariant";


	//AManagerHUD* Hud = Cast<AManagerHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());



	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("buff", m_Name);
	JsonObject->SetStringField("group", m_Group);
	JsonObject->SetStringField("group", m_Group);
	JsonObject->SetStringField("group", m_Group);
	JsonObject->SetStringField("group", m_Group);
	JsonObject->SetStringField("group", m_Group);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::CreateTestVariantReceive);
	Request->SetVerb("POST");
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->ProcessRequest();

}

void AManagerHUD::CreateTestVariantReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		//FString answer = Response->GetContentAsString();
		OnTestVariantCreated_Callback.Broadcast(true);
	}
}

void AManagerHUD::GetAllQuestionsSend(FString Id)
{
	FString OutputString;
	FString URL = "http://" + Config::SERVER_IP + "/getallvariantdata";
	FString variant_id = Id;
	URL = URL + "?variant_id=" + variant_id;

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetAllQuestionsReceive);
	Request->SetVerb("GET");
	Request->SetURL(URL);
	Request->ProcessRequest();
}

void AManagerHUD::GetAllQuestionsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TArray<FTestDataArrayStruct> Questions;
	//Questions.Empty();

	if (bWasSuccessful)
	{
		TArray<TSharedPtr<FJsonValue>> JsonArray;
		FString answer = Response->GetContentAsString();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, answer);

		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //������� JSON ������ �������
		//TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
		FJsonSerializer::Deserialize(JsonReader, JsonArray);




		int counter = 0;


		for (const TSharedPtr<FJsonValue>& JsonValue : JsonArray)
		{
			TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();
			FTestDataArrayStruct PulledTestData;


			FString Question = JsonObject->GetStringField("question_text");
			int32 QuestionType = JsonObject->GetNumberField("question_type");
			FString Answer_1 = JsonObject->GetStringField("answer_1");
			FString Answer_2 = JsonObject->GetStringField("answer_2");
			FString Answer_3 = JsonObject->GetStringField("answer_3");
			FString Answer_4 = JsonObject->GetStringField("answer_4");
			FString Correct_Answer = JsonObject->GetStringField("answer_correct");



			PulledTestData.QuestionText = Question;
			PulledTestData.QuestionType = QuestionType;
			PulledTestData.Answer_1 = Answer_1;
			PulledTestData.Answer_2 = Answer_2;
			PulledTestData.Answer_3 = Answer_3;
			PulledTestData.Answer_4 = Answer_4;
			PulledTestData.CorrectAnswer = Correct_Answer;
			Questions.Add(PulledTestData);


			UE_LOG(LogTemp, Warning, TEXT("%s"), *Question);
			UE_LOG(LogTemp, Warning, TEXT("%d"), QuestionType);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Answer_1);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Answer_2);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Answer_3);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Answer_4);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Correct_Answer);

			counter++;
		}
		OnAllQuestionsReceived_Callback.Broadcast(true, Questions);
	}
	else
	{
		//		FOnAllQuestionsReveived_Callback.Broadcast(false);
	}
}

void AManagerHUD::UpdateQuestionsSend(FString Id, TArray<FTestDataArrayStruct> Array)
{
	FString OutputString;
	FString StringToSend;

	TSharedPtr<FJsonObject> JsonToSend = MakeShareable(new FJsonObject());
	TArray<TSharedPtr<FJsonValue>> JsonArray;

	for (int i = 0; i <= (Array.Num() - 1); i++)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		JsonObject->SetStringField("question_text", Array[i].QuestionText);
		JsonObject->SetNumberField("question_type", Array[i].QuestionType);
		JsonObject->SetStringField("answer_1", Array[i].Answer_1);
		JsonObject->SetStringField("answer_2", Array[i].Answer_2);
		JsonObject->SetStringField("answer_3", Array[i].Answer_3);
		JsonObject->SetStringField("answer_4", Array[i].Answer_4);
		JsonObject->SetStringField("answer_correct", Array[i].CorrectAnswer);
		JsonObject->SetNumberField("id_variant", FCString::Atoi(*Id));

		JsonArray.Add(MakeShareable(new FJsonValueObject(JsonObject)));
	}

	JsonToSend->SetNumberField("variant_id", FCString::Atoi(*Id)); //Ð”ÐžÐ‘ÐÐ’Ð›Ð•ÐÐ˜Ð• ÐŸÐžÐ›Ð¯ Ð’ÐÐ Ð˜ÐÐÐ¢Ð Ð’ JSON ÐšÐžÐ¢ÐžÐ Ð«Ð™ Ð‘Ð£Ð”Ð•Ð¢ ÐžÐ¢ÐŸÐ ÐÐ’Ð›Ð•Ð (ÐÐÐ”Ðž ÐŸÐžÐœÐ•ÐÐ¯Ð¢Ð¬ ÐÐ ÐŸÐ•Ð Ð•ÐœÐ•ÐÐÐ£Ð®, Ð ÐÐ• ÐÐ Ð¡Ð¢ÐÐ¢Ð˜Ð§Ð•Ð¡ÐšÐžÐ• Ð—ÐÐÐ§Ð•ÐÐ˜Ð• 8)
	JsonToSend->SetArrayField("items", JsonArray); //Ð”ÐžÐ‘ÐÐ’Ð›Ð•ÐÐ˜Ð• ÐœÐÐ¡Ð¡Ð˜Ð’Ð JSON Ð’ JSON ÐšÐžÐ¢ÐžÐ Ð«Ð™ Ð‘Ð£Ð”Ð•Ð¢ ÐžÐ¢ÐŸÐ ÐÐ’Ð›Ð•Ð

	TSharedRef<TJsonWriter<>> Writer2 = TJsonWriterFactory<>::Create(&StringToSend); //Ð‘Ð›Ð Ð‘Ð›Ð Ð‘Ð›Ð, Ð”ÐÐ›Ð•Ð• Ð’Ð¡Ð• ÐšÐÐš Ð˜ Ð ÐÐÐ¬Ð¨Ð•
	FJsonSerializer::Serialize(JsonToSend.ToSharedRef(), Writer2);

	FString URL = "http://" + Config::SERVER_IP + "/updatevariantquestions";
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::UpdateQuestionsReceive);
	Request->SetVerb("POST");
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(StringToSend);
	Request->ProcessRequest();
}

void AManagerHUD::UpdateQuestionsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FString answer = Response->GetContentAsString();
		if (answer.Equals("1"))
		{
			OnQuestionUpdated_Callback.Broadcast(true);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Successfuly updated"));
			
		}
	}
	else
	{
		//		FOnQuestionsUpdated_Callback.Broadcast(false);
	}
}


  void AManagerHUD::GetSimVariantsIdsSend()// Код Максима
{
	FString group = GroupName;
	uint32 user_id = UserID;

	FString URL = "http://" + Config::SERVER_IP + "/getallusvariantsdata";
	FString OutputString;

  TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("group", group);
	JsonObject->SetNumberField("user_id", user_id);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
  
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetSimVariantsIdsReceive);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->SetURL(URL);
	Request->ProcessRequest();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, group);
}// Код Максима

void AManagerHUD::GetSimVariantsIdsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)// Код Максима
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
		OnSimVariantIdsReceived_Callback.Broadcast(true, SimArrayIds, CompletedSimArrayIds);
	}
	else
	{
		OnSimVariantIdsReceived_Callback.Broadcast(false, SimArrayIds, CompletedSimArrayIds);
	}
}// Код Максима



void AManagerHUD::CreateSimVariantSend(FString m_VariantName)
{
	FString group = GroupName;
	FString VariantName = m_VariantName;

	FString URL = "http://" + Config::SERVER_IP + "/createemptyusvariant";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("group", group);
	JsonObject->SetStringField("usvariant_name", VariantName);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::CreateSimVariantRecive);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->SetURL(URL);
	Request->ProcessRequest();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, group);
}

void AManagerHUD::CreateSimVariantRecive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	FString answer = Response->GetContentAsString();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, answer);

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ JSON ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	//TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	FJsonSerializer::Deserialize(JsonReader, JsonArray);




	FString callback_message = answer;



	if (callback_message == "1")
	{
		OnSimVariantCreated_Callback.Broadcast(true);
	}
	else
	{
		OnSimVariantCreated_Callback.Broadcast(false);
	}

}


void AManagerHUD::DeleteVariantSend(int32 m_Var_ID)
{
	uint32 Variant_ID = m_Var_ID;


	FString URL = "http://" + Config::SERVER_IP + "/deleteusvariant";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetNumberField("usvariant_id", Variant_ID);
	//JsonObject->SetStringField("usvariant_name", VariantName);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::DeleteVariantRecive);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->SetURL(URL);
	Request->ProcessRequest();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::FromInt(Variant_ID));
}

void AManagerHUD::DeleteVariantRecive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	FString answer = Response->GetContentAsString();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, answer);

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ JSON ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	//TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	FJsonSerializer::Deserialize(JsonReader, JsonArray);




	FString callback_message = answer;



	if (callback_message == "1")
	{
		OnSimVariantDeleted_Callback.Broadcast(true);
	}
	else
	{
		OnSimVariantDeleted_Callback.Broadcast(false);
	}

}

//uint32 Variant_ID = m_Var_ID;
//
//
//FString URL = "http://" + Config::SERVER_IP + "/deleteusvariant";
//FString OutputString;
//
//TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
//JsonObject->SetNumberField("usvariant_id", Variant_ID);
////JsonObject->SetStringField("usvariant_name", VariantName);
//TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
//FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
//
//TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
//Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::DeleteVariantRecive);
//Request->SetVerb("POST");
//Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
//Request->SetContentAsString(OutputString);
//Request->SetURL(URL);
//Request->ProcessRequest();
//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::FromInt(Variant_ID));

void AManagerHUD::AssignEmployeeSend(int32 m_VarID, int32 m_EmployeeID)
{
	FString URL = "http://" + Config::SERVER_IP + "/addworkertovariant";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetNumberField("worker_id", m_EmployeeID);
	JsonObject->SetNumberField("usvariant_id", m_VarID);

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::AssignEmployeeReceive);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->SetURL(URL);
	Request->ProcessRequest();

}

void AManagerHUD::AssignEmployeeReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnEmployeeAssigned_Callback.Broadcast(true);
	}
}

void AManagerHUD::RemoveEmployeeSend(int32 m_VarID, int32 m_EmployeeID)
{
	FString URL = "http://" + Config::SERVER_IP + "/deleteworkerfromvariant";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetNumberField("worker_id", m_EmployeeID);
	JsonObject->SetNumberField("usvariant_id", m_VarID);

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::RemoveEmployeeReceive);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->SetURL(URL);
	Request->ProcessRequest();
}

void AManagerHUD::RemoveEmployeeReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnEmployeeRemoved_Callback.Broadcast(true);
	}
}

void AManagerHUD::AssignUserStorySend(int32 m_VarID, int32 m_UserStoryID)
{
	FString URL = "http://" + Config::SERVER_IP + "/addustovariant";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetNumberField("US_id", m_UserStoryID);
	JsonObject->SetNumberField("usvariant_id", m_VarID);

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::AssignUserStoryReceive);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->SetURL(URL);
	Request->ProcessRequest();
}

void AManagerHUD::AssignUserStoryReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnUserStoryAssigned_Callback.Broadcast(true);
	}
}

void AManagerHUD::RemoveUserStorySend(int32 m_VarID, int32 m_UserStoryID)
{
	FString URL = "http://" + Config::SERVER_IP + "/deleteusfromvariant";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetNumberField("US_id", m_UserStoryID);
	JsonObject->SetNumberField("usvariant_id", m_VarID);

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::RemoveUserStoryReceive);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->SetURL(URL);
	Request->ProcessRequest();
}

void AManagerHUD::RemoveUserStoryReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnUserStoryRemoved_Callback.Broadcast(true);
	}
}



void AManagerHUD::GetModifiersSend()
{
	FString OutputString;
	FString URL = "http://" + Config::SERVER_IP + "/getmodifiers";

	//URL = URL + "?usvariant_id=" + variant_id;

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetModifiersReceive);
	Request->SetVerb("GET");
	Request->SetURL(URL);
	Request->ProcessRequest();
}

void AManagerHUD::GetModifiersReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
		FString answer = Response->GetContentAsString();
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer);
		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			// Get Modifiers

		//	const TArray<TSharedPtr<FJsonValue>>* ModifiersArray;
		
			TArray<FModifierData> Buffs;
			TArray<FModifierData> Debuffs;


			const TArray<TSharedPtr<FJsonValue>>* BuffsArray;
			if (JsonObject->TryGetArrayField("buffs", BuffsArray))
			{


				for (const TSharedPtr<FJsonValue>& ModifierValue : *BuffsArray)
				{
					const TSharedPtr<FJsonObject> ModifierObj = ModifierValue->AsObject();
					if (ModifierObj.IsValid())
					{
						FModifierData Buff;
						Buff.ID = ModifierObj->GetIntegerField("buff_id");
						Buff.Name = ModifierObj->GetStringField("buff_name");
						Buff.Description = ModifierObj->GetStringField("buff_description");
						Buff.USCompleteChance = ModifierObj->GetNumberField("buff_USCompleteChance");
						Buff.MaxSPModificator = ModifierObj->GetNumberField("buff_MaxSPModificator");
						Buff.MaxHoursModificator = ModifierObj->GetNumberField("buff_MaxHoursModificator");
						Buff.MaxTasksModificator = ModifierObj->GetNumberField("buff_MaxTasksModificator");
						Buff.Type = 1;
						Buffs.Add(Buff);
						
					}
				}
			}

			const TArray<TSharedPtr<FJsonValue>>* DebuffsArray;
			if (JsonObject->TryGetArrayField("debuffs", DebuffsArray))
			{
	

				for (const TSharedPtr<FJsonValue>& ModifierValue : *DebuffsArray)
				{
					const TSharedPtr<FJsonObject> ModifierObj = ModifierValue->AsObject();
					if (ModifierObj.IsValid())
					{
						FModifierData Debuff;
						Debuff.ID = ModifierObj->GetIntegerField("debuff_id");
						Debuff.Name = ModifierObj->GetStringField("debuff_name");
						Debuff.Description = ModifierObj->GetStringField("debuff_description");
						Debuff.USCompleteChance = ModifierObj->GetNumberField("debuff_USCompleteChance");
						Debuff.MaxSPModificator = ModifierObj->GetNumberField("debuff_MaxSPModificator");
						Debuff.MaxHoursModificator = ModifierObj->GetNumberField("debuff_MaxHoursModificator");
						Debuff.MaxTasksModificator = ModifierObj->GetNumberField("debuff_MaxTasksModificator");
						Debuff.Type = 0;
						Debuffs.Add(Debuff);
					}
				}
			}


			OnModifiersDataReceived_Callback.Broadcast(true, Buffs, Debuffs);
			
		}

	}
}

void AManagerHUD::CreateModifierSend(FModifierData Data, int32 Type)
{

	FString URL = "http://" + Config::SERVER_IP + "/createmodifier";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("m_name", Data.Name);
	JsonObject->SetStringField("m_description", Data.Description);
	JsonObject->SetNumberField("m_USCompleteChance", Data.USCompleteChance);
	JsonObject->SetNumberField("m_MaxSPModificator", Data.MaxSPModificator);
	JsonObject->SetNumberField("m_MaxHoursModificator", Data.MaxHoursModificator);
	JsonObject->SetNumberField("m_MaxTasksModificator", Data.MaxTasksModificator);
	JsonObject->SetNumberField("flag", Type);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::CreateModifierReceive);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->SetURL(URL);
	Request->ProcessRequest();

}

void AManagerHUD::CreateModifierReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnModifierCreated_Callback.Broadcast(true);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("--Modifier Created--"));
	}
	
}


void AManagerHUD::DeleteModifierSend(int32 m_ID, int32 Type)
{
	uint32 ModifierID = m_ID;

	FString URL = "http://" + Config::SERVER_IP + "/deletemodifier";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetNumberField("m_id", ModifierID);
	JsonObject->SetNumberField("flag", Type);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::DeleteModifierReceive);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->SetURL(URL);
	Request->ProcessRequest();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::FromInt(m_ID));
}

void AManagerHUD::DeleteModifierReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnModifierDeleted_Callback.Broadcast(true);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("--Modifier Deleted--"));
	}
}

void AManagerHUD::UpdateModifierSend(int32 m_ID, FModifierData Data)
{
	uint32 ModifierID = m_ID;

	FString URL = "http://" + Config::SERVER_IP + "/updatemodifier";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetNumberField("m_id", m_ID);
	JsonObject->SetStringField("m_name", Data.Name);
	JsonObject->SetStringField("m_description", Data.Description);
	JsonObject->SetNumberField("m_USCompleteChance", Data.USCompleteChance);
	JsonObject->SetNumberField("m_MaxSPModificator", Data.MaxSPModificator);
	JsonObject->SetNumberField("m_MaxHoursModificator", Data.MaxHoursModificator);
	JsonObject->SetNumberField("m_MaxTasksModificator", Data.MaxTasksModificator);
	JsonObject->SetNumberField("flag", Data.Type);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::UpdateModifierReceive);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->SetURL(URL);
	Request->ProcessRequest();
}

void AManagerHUD::UpdateModifierReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnModifierUpdated_Callback.Broadcast(true);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("--Modifier Updated--"));
	}
}


void AManagerHUD::GetSimVariantDataSend(int32 m_Var_ID)
{
	FString OutputString;
	FString URL = "http://" + Config::SERVER_IP + "/getusvariantdata";



	FString variant_id = FString::FromInt(m_Var_ID);
	URL = URL + "?usvariant_id=" + variant_id;

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetSimVariantDataReceive);
	Request->SetVerb("GET");
	Request->SetURL(URL);
	Request->ProcessRequest();
}

void AManagerHUD::GetSimVariantDataReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{


	SimVariantData.ID = -1;
	SimVariantData.Employees.Empty();
	SimVariantData.UserStories.Empty();

	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	FString answer = Response->GetContentAsString();

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ JSON ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%s"), *answer));

	//FFileHelper::SaveStringToFile(*answer, *FString("C:/file.txt"));

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{


		int32 Var_ID = JsonObject->GetIntegerField("usvariant_id");
		FString Var_Name = JsonObject->GetStringField("name");
		int32 Var_Days = JsonObject->GetIntegerField("days");
		int32 Var_Sprints = JsonObject->GetIntegerField("sprints");


		SimVariantData.ID = Var_ID;
		SimVariantData.Name = Var_Name;
		SimVariantData.Days = Var_Days;
		SimVariantData.Sprints = Var_Sprints;


		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%s"), *Var_Name));




		// Get Employees Data
		const TArray<TSharedPtr<FJsonValue>>* EmployeesArray;
		if (JsonObject->TryGetArrayField("workers", EmployeesArray))
		{
			for (const TSharedPtr<FJsonValue>& EmployeeValue : *EmployeesArray)
			{
				const TSharedPtr<FJsonObject> EmployeeObj = EmployeeValue->AsObject();
				if (EmployeeObj.IsValid())
				{
					FEmployeeData Employee;


					int32 EmployeeID = EmployeeObj->GetIntegerField("worker_id");
					FString Employee_Name = EmployeeObj->GetStringField("worker_name");
					FString Employee_Description = EmployeeObj->GetStringField("worker_description");
					int32 Employee_MaxHours = EmployeeObj->GetIntegerField("worker_maxhours");
					int32 Employee_MaxTasks = EmployeeObj->GetIntegerField("worker_maxtasks");
					int32 Employee_Skill = EmployeeObj->GetIntegerField("worker_maxsp");
					float Employee_Prof_3D = EmployeeObj->GetNumberField("worker_3D");
					float Employee_Prof_2D = EmployeeObj->GetNumberField("worker_2D");
					float Employee_Prof_Code = EmployeeObj->GetNumberField("worker_code");



					// Get Modifiers

					// Buffs
					const TArray<TSharedPtr<FJsonValue>>* BuffsArray;
					if (EmployeeObj->TryGetArrayField("buffs", BuffsArray))
					{
						for (const TSharedPtr<FJsonValue>& ModifierValue : *BuffsArray)
						{
							const TSharedPtr<FJsonObject> ModifierObj = ModifierValue->AsObject();
							if (ModifierObj.IsValid())
							{
								FModifierData Buff;
								Buff.ID = ModifierObj->GetIntegerField("buff_id");
								Buff.Name = ModifierObj->GetStringField("buff_name");
								Buff.Description = ModifierObj->GetStringField("buff_description");
								Buff.USCompleteChance = ModifierObj->GetNumberField("buff_USCompleteChance");
								Buff.MaxSPModificator = ModifierObj->GetNumberField("buff_MaxSPModificator");
								Buff.MaxHoursModificator = ModifierObj->GetNumberField("buff_MaxHoursModificator");
								Buff.MaxTasksModificator = ModifierObj->GetNumberField("buff_MaxTasksModificator");

								// Add to struct here
								Employee.Buffs.Add(Buff);

							}
						}
					}


					// Debuffs
					const TArray<TSharedPtr<FJsonValue>>* DebuffsArray;
					if (EmployeeObj->TryGetArrayField("debuffs", DebuffsArray))
					{
						for (const TSharedPtr<FJsonValue>& ModifierValue : *DebuffsArray)
						{
							const TSharedPtr<FJsonObject> ModifierObj = ModifierValue->AsObject();
							if (ModifierObj.IsValid())
							{
								FModifierData Debuff;
								Debuff.ID = ModifierObj->GetIntegerField("debuff_id");
								Debuff.Name = ModifierObj->GetStringField("debuff_name");
								Debuff.Description = ModifierObj->GetStringField("debuff_description");
								Debuff.USCompleteChance = ModifierObj->GetNumberField("debuff_USCompleteChance");
								Debuff.MaxSPModificator = ModifierObj->GetNumberField("debuff_MaxSPModificator");
								Debuff.MaxHoursModificator = ModifierObj->GetNumberField("debuff_MaxHoursModificator");
								Debuff.MaxTasksModificator = ModifierObj->GetNumberField("debuff_MaxTasksModificator");

								// Add to struct here
								Employee.Debuffs.Add(Debuff);
							}
						}
					}

					Employee.ID = EmployeeID;
					Employee.Name = Employee_Name;
					Employee.Description = Employee_Description;
					Employee.MaxHours = Employee_MaxHours;
					Employee.MaxTasks = Employee_MaxTasks;
					Employee.Skill = Employee_Skill;
					Employee.Proficiency.Modeling = Employee_Prof_3D;
					Employee.Proficiency.Art = Employee_Prof_2D;
					Employee.Proficiency.Code = Employee_Prof_Code;

					// Add Employee to sim data array
					SimVariantData.Employees.Add(Employee);

				}
			}
		}


		// Get UserStories Data
		const TArray<TSharedPtr<FJsonValue>>* UserStoriesArray;
		if (JsonObject->TryGetArrayField("UserStories", UserStoriesArray))
		{
			for (const TSharedPtr<FJsonValue>& StoryValue : *UserStoriesArray)
			{
				const TSharedPtr<FJsonObject> StoryObj = StoryValue->AsObject();
				if (StoryObj.IsValid())
				{
					FUSData USData;

					int32 US_ID = StoryObj->GetIntegerField("us_id");
					FString US_Description = StoryObj->GetStringField("us_description");
					int32 US_Complexity = StoryObj->GetIntegerField("us_compexity");
					int32 US_Hours = StoryObj->GetIntegerField("us_hours");


					//  Get DoBefore Array
					const TArray<TSharedPtr<FJsonValue>>* DoBeforeArray;
					if (StoryObj->TryGetArrayField("us_dobefore", DoBeforeArray))
					{
						for (const TSharedPtr<FJsonValue>& Value : *DoBeforeArray)
						{
							int32 ID = Value->AsNumber();
							USData.DoBefore.Add(ID);
						}
					}
					//  Get Children Array
					const TArray<TSharedPtr<FJsonValue>>* DoughterArray;
					if (StoryObj->TryGetArrayField("us_doughter", DoughterArray))
					{
						for (const TSharedPtr<FJsonValue>& Value : *DoughterArray)
						{
							USData.ChildUS.Add(Value->AsNumber());
						}
					}
					//  Get Parents Array
					const TArray<TSharedPtr<FJsonValue>>* ParentArray;
					if (StoryObj->TryGetArrayField("us_parent", ParentArray))
					{
						for (const TSharedPtr<FJsonValue>& Value : *ParentArray)
						{
							USData.ParentUS.Add(Value->AsNumber());
						}
					}



					bool US_3D = StoryObj->GetBoolField("3D");
					bool US_2D = StoryObj->GetBoolField("2D");
					bool US_Code = StoryObj->GetBoolField("Code");

					USData.ID = US_ID;
					USData.Description = US_Description;
					USData.Complexity = US_Complexity;
					USData.Hours = US_Hours;
					USData.Proficiencies.Modeling = US_3D;
					USData.Proficiencies.Art = US_2D;
					USData.Proficiencies.Code = US_Code;

					// Add userstories array to variant data
					SimVariantData.UserStories.Add(USData);


					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%s"), *SimVariantData.Name));
				}
			}
		}

		//		SimVariantData = NewSimVariantData;
	}

	if (bWasSuccessful)
	{
		OnSimVariantDataReceived_Callback.Broadcast(true, SimVariantData);
	}
	else
	{
		OnSimVariantDataReceived_Callback.Broadcast(false, SimVariantData);
	}
}
