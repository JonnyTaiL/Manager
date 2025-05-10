// Fill out your copyright notice in the Description page of Project Settings.


#include "W_SimWindowBody.h"
#include "Manager/ManagerHUD.h"
#include "Json.h"
#include "Kismet/GameplayStatics.h"

void UW_SimWindowBody::NativeConstruct()
{
	GetAllUsVariantsSend();
}

void UW_SimWindowBody::GetAllUsVariantsSend()
{
	AHUD* Hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD();
	AManagerHUD* MHud = Cast<AManagerHUD>(Hud);
	FString group = MHud->GroupName;
	uint32 user_id = MHud->UserID; //FString::FromInt(MHud->UserID);

	FString URL = "http://26.76.184.253:8000/getallusvariantsdata";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("group", group);
	JsonObject->SetNumberField("user_id", user_id);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UW_SimWindowBody::GetAllUsVariantsReceive);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->SetURL(URL);
	Request->ProcessRequest();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, group);
}

void UW_SimWindowBody::GetAllUsVariantsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	SimVariantIds.Empty();
	CompletedSimArrayIds.Empty();
	SimArrayIds.Empty();

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	FString answer = Response->GetContentAsString();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, answer);

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //������� JSON ������ �������
	//TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	FJsonSerializer::Deserialize(JsonReader, JsonArray);

	uint32 counter = 0;


	for (const TSharedPtr<FJsonValue>& JsonValue : JsonArray)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, answer);
		TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();
		FString var_id = JsonObject->GetStringField("usvariant_id");
		uint32 completed = JsonObject->GetNumberField("iscompleted_by_user");

		
		SimArrayIds.Add(var_id);
		if (completed == 1)
		{
			CompletedSimArrayIds.Add(var_id);
		}

		GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Blue, SimArrayIds[counter]);


		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, answer);

		counter++;
	}
}

void UW_SimWindowBody::CreateEmptyUsVariantSend(FString m_VariantName, FString m_Group)
{
	AHUD* Hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD();
	AManagerHUD* MHud = Cast<AManagerHUD>(Hud);

	FString group = m_Group;
	FString VariantName = m_VariantName;
	
	FString URL = "http://26.76.184.253:8000/createemptyusvariant";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("group", group);
	JsonObject->SetStringField("usvariant_name", VariantName);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UW_SimWindowBody::GetAllUsVariantsReceive);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->SetURL(URL);
	Request->ProcessRequest();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, group);
}

void UW_SimWindowBody::CreateEmptyUsVariantRecive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	FString answer = Response->GetContentAsString();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, answer);

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //������� JSON ������ �������
	//TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	FJsonSerializer::Deserialize(JsonReader, JsonArray);




	FString callback_message = answer;

		

	if (callback_message == "1")
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Add Simulation Variant - Success"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Add Simulation Variant - Error"));
	}
	
}

void UW_SimWindowBody::DeleteVariantSend(int32 m_Var_ID)
{
	AHUD* Hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD();
	AManagerHUD* MHud = Cast<AManagerHUD>(Hud);

	uint32 Variant_ID = m_Var_ID;


	FString URL = "http://26.76.184.253:8000/deleteusvariant";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetNumberField("usvariant_id", Variant_ID);
	//JsonObject->SetStringField("usvariant_name", VariantName);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UW_SimWindowBody::GetAllUsVariantsReceive);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->SetURL(URL);
	Request->ProcessRequest();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::FromInt(Variant_ID));
}

void UW_SimWindowBody::DeleteVariantRecive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	FString answer = Response->GetContentAsString();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, answer);

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //������� JSON ������ �������
	//TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	FJsonSerializer::Deserialize(JsonReader, JsonArray);




	FString callback_message = answer;



	if (callback_message == "1")
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Delete Simulation Variant - Success"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Delete Simulation Variant - Error"));
	}
	
}

void UW_SimWindowBody::GetVariantDataSend(int32 m_Var_ID)
{
	FString OutputString;
	FString URL = "http://26.76.184.253:8000/getusvariantdata";



	FString variant_id = FString::FromInt(m_Var_ID);
	URL = URL + "?usvariant_id=" + variant_id;

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UW_SimWindowBody::GetVariantDataReceive);
	Request->SetVerb("GET");
	Request->SetURL(URL);
	Request->ProcessRequest();
}

void UW_SimWindowBody::GetVariantDataReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{


	SimVariantData.ID = -1;
	SimVariantData.Employees.Empty();
	SimVariantData.UserStories.Empty();

	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	FString answer = Response->GetContentAsString();

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //������� JSON ������ �������

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
					FString EmployeeName = EmployeeObj->GetStringField("worker_name");
					int32 Employee_MaxHours = EmployeeObj->GetIntegerField("worker_maxhours");
					int32 Employee_MaxTasks = EmployeeObj->GetIntegerField("worker_maxsp");
					float Employee_Prof_3D = EmployeeObj->GetNumberField("worker_3D");
					float Employee_Prof_2D = EmployeeObj->GetNumberField("worker_2D");
					float Employee_Prof_Code = EmployeeObj->GetNumberField("worker_code");

				
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Employee Added");

					// Get Modifiers

					// Buffs
					const TArray<TSharedPtr<FJsonValue>>* BuffsArray;
					if (EmployeeObj->TryGetArrayField("modifiers", BuffsArray))
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

								// Add to struct here
								Employee.Buffs.Add(Buff);
						
							}
						}
					}


					// Debuffs
					const TArray<TSharedPtr<FJsonValue>>* DebuffsArray;
					if (EmployeeObj->TryGetArrayField("modifiers", DebuffsArray))
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

								// Add to struct here
								Employee.Debuffs.Add(Debuff);
							}
						}
					}
	
					Employee.ID = EmployeeID;
					Employee.Name = EmployeeName;
					Employee.MaxHours = Employee_MaxHours;
					Employee.MaxTasks = Employee_MaxTasks;
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
					FString US_Descrioption = StoryObj->GetStringField("us_description");
					int32 US_Complexity = StoryObj->GetIntegerField("us_compexity");
					int32 US_Hours = StoryObj->GetIntegerField("us_hours");


					//  Get DoBefore Array
					const TArray<TSharedPtr<FJsonValue>>* DoBeforeArray;
					if (StoryObj->TryGetArrayField("us_dobefore", DoBeforeArray))
					{
						for (const TSharedPtr<FJsonValue>& Value : *DoBeforeArray)
						{
							int32 ID = Value->AsNumber(); // or .AsInteger() if using UE 5.3+
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
					USData.Description = US_Descrioption;
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
}
