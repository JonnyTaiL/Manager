// Fill out your copyright notice in the Description page of Project Settings.


#include "W_SimWindowBody.h"
#include "Manager/ManagerHUD.h"
#include "Json.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/ManagerConfig.h"


void UW_SimWindowBody::NativeConstruct()
{
}


void UW_SimWindowBody::CreateEmptyUsVariantSend(FString m_VariantName, FString m_Group)
{
	AHUD* Hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD();
	AManagerHUD* MHud = Cast<AManagerHUD>(Hud);

	FString group = m_Group;
	FString VariantName = m_VariantName;
	
	FString URL = "http://" + Config::SERVER_IP + "/createemptyusvariant";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("group", group);
	JsonObject->SetStringField("usvariant_name", VariantName);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UW_SimWindowBody::CreateEmptyUsVariantRecive);
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


	FString URL = "http://" + Config::SERVER_IP + "/deleteusvariant";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetNumberField("usvariant_id", Variant_ID);
	//JsonObject->SetStringField("usvariant_name", VariantName);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UW_SimWindowBody::DeleteVariantRecive);
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
	FString URL = "http://" + Config::SERVER_IP + "/getusvariantdata";



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
}
