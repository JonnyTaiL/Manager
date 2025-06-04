// Fill out your copyright notice in the Description page of Project Settings.


#include "W_TestsWindowBody.h"
#include "Engine.h"
#include "Json.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/Interfaces/IHUDRequestData.h"
#include "Manager/ManagerHUD.h"
#include "Manager/ManagerTypes.h"


void UW_TestsWindowBody::NativeConstruct()
{

	//GetVariantsDataSend();


}

void UW_TestsWindowBody::GetVariantsDataSend()
{

	AHUD* Hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD();
	AManagerHUD* MHud = Cast<AManagerHUD>(Hud);
	FString group = MHud->GroupName;

	if (MHud != nullptr)
	{
		FString URL = "http://26.76.184.253:8000/getvariantsdata";
		FString OutputString;

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		JsonObject->SetStringField("group", group);
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

		TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetVariantsDataReceive);
		Request->SetVerb("POST");
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		Request->SetURL(URL);
		Request->SetContentAsString(OutputString);
		Request->ProcessRequest();

	}

}

void UW_TestsWindowBody::GetVariantsDataReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	TestsArrayIds.Empty();

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

		FOnVariantDataReceived_Callback.Broadcast(true);
	}
	else
	{
		FOnVariantDataReceived_Callback.Broadcast(false);
	}
	
	
}

void UW_TestsWindowBody::GetTestDataArraySend(FString Id)
{
	FString URL = "http://26.76.184.253:8000/getallvariantdata";
	FString variant_id = Id; 
	URL = URL + "?variant_id=" + variant_id;



	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetTestDataArrayRecieve);
	Request->SetVerb("GET");
	Request->SetURL(URL);
	Request->ProcessRequest();
}

void UW_TestsWindowBody::GetTestDataArrayRecieve(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	if (bWasSuccessful)
	{
		TArray<TSharedPtr<FJsonValue>> JsonArray;
		FString answer = Response->GetContentAsString();

		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //ПАРСИНГ JSON ОТВЕТА Массива
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
			TestDataArray.Add(PulledTestData);


			UE_LOG(LogTemp, Warning, TEXT("%s"), *Question);
			UE_LOG(LogTemp, Warning, TEXT("%d"), QuestionType);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Answer_1);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Answer_2);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Answer_3);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Answer_4);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Correct_Answer);

			counter++;
		}

		FOnTestDataArrayReceived_Callback.Broadcast(true);
	}
	else
	{
		FOnTestDataArrayReceived_Callback.Broadcast(false);
	}
	
}

void UW_TestsWindowBody::AddCompletedVariantSend(int32 UserId, int32 VarID, int32 Score)
{
	FString URL = "http://26.76.184.253:8000/addcompletedvariants";
	FString OutputString;



	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetNumberField("user_id", UserId);
	JsonObject->SetNumberField("variant_id", VarID);
	JsonObject->SetNumberField("score", Score);

	

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);







	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetTestDataArrayRecieve);
	Request->SetVerb("POST");
	Request->SetURL(URL);

	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->ProcessRequest();

}

void UW_TestsWindowBody::AddCompletedVariantRecieve(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

}

void UW_TestsWindowBody::GetCompletedVariantsDataSend(FString Id)
{
	FString URL = "http://26.76.184.253:8000/getcompletedvariantsdata";
	FString variant_id = Id;
	URL = URL + "?user_id=" + variant_id;



	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetCompletedVariantsDataReceive);
	Request->SetVerb("GET");
	Request->SetURL(URL);
	Request->ProcessRequest();
}

void UW_TestsWindowBody::GetCompletedVariantsDataReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	
	CompletedTestsArrayIds.Empty();

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
			FOnCompletedVariantsReceived_Callback.Broadcast(true);
		}

	}
	else
	{
		FOnCompletedVariantsReceived_Callback.Broadcast(false);
	}
}

void UW_TestsWindowBody::QuestionSend(FString UserAnswer, FString CorrectAnswer)
{
	
		FString OutputString;
		
		FString URL = "http://26.76.184.253:8001/compare_answers";
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		JsonObject->SetStringField("UserAnswer", UserAnswer);
		JsonObject->SetStringField("CorrectAnswer", CorrectAnswer);
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

		TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
		Request->OnProcessRequestComplete().BindUObject(this, &UW_TestsWindowBody::QuestionGet);
		Request->SetVerb("POST");
		Request->SetURL(URL);
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		Request->SetContentAsString(OutputString);
		Request->ProcessRequest();
}

void UW_TestsWindowBody::QuestionGet(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FString answer = Response->GetContentAsString();
		AnswerRating = answer;
		FOnQuestionReceiced_Callback.Broadcast(true);
	}
	else
	{
		FOnQuestionReceiced_Callback.Broadcast(false);
	}

}


void UW_TestsWindowBody::CreateVariantSend(FString Name, FString Group)
{
	FString OutputString;
	FString URL = "http://26.76.184.253:8000/createvariant";


	//AManagerHUD* Hud = Cast<AManagerHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	


	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("variant_name", TB_VarName->TB_TextBox->GetText().ToString());
	JsonObject->SetStringField("group", TB_Group->TB_TextBox->GetText().ToString());
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::CreateVariantReceive);
	Request->SetVerb("POST");
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->ProcessRequest();

}

void UW_TestsWindowBody::CreateVariantReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FString answer = Response->GetContentAsString();
		AnswerRating = answer;
		FOnVariantCreated_Callback.Broadcast(true);
	}
}





void UW_TestsWindowBody::GetAllQuestionsSend(FString Id)
{
	FString OutputString;
	FString URL = "http://26.76.184.253:8000/getallvariantdata";
	FString variant_id = Id;
	URL = URL + "?variant_id=" + variant_id;

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetAllQuestionsReceive);
	Request->SetVerb("GET");
	Request->SetURL(URL);
	Request->ProcessRequest();
}


void UW_TestsWindowBody::GetAllQuestionsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	Questions.Empty();

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
		FOnAllQuestionsReveived_Callback.Broadcast(true);
	}
	else
	{
		FOnAllQuestionsReveived_Callback.Broadcast(false);
	}
	
}

void UW_TestsWindowBody::UpdateQuestionsSend(FString Id, TArray<FTestDataArrayStruct> Array)
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

	FString URL = "http://26.76.184.253:8000/updatevariantquestions";
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UW_TestsWindowBody::UpdateQuestionsReceive);
	Request->SetVerb("POST");
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(StringToSend);
	Request->ProcessRequest();
}

void UW_TestsWindowBody::UpdateQuestionsReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FString answer = Response->GetContentAsString();
		if (answer.Equals("1"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Successfuly updated"));
			FOnQuestionsUpdated_Callback.Broadcast(true);
		}
	}
	else
	{
		FOnQuestionsUpdated_Callback.Broadcast(false);
	}

}








//
//void UW_TestsWindowBody::CreateVariantReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
//{
//	FString answer = Response->GetContentAsString();
//
//	AHUD* Hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD();
//	FManagerUserData User = Cast<IIHUDRequestData>(Hud)->GetUserData_Implementation();
//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, User.Name);
//
//	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //ПАРСИНГ JSON ОТВЕТА И ПОЛУЧЕНИЕ ПОЛЯ ГРУППЫ ЧЕЛОВЕКА
//	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
//	FJsonSerializer::Deserialize(JsonReader, JsonObject);
//
//	FString UserID = JsonObject->GetNumberField("variant_name");
//	FString login = JsonObject->GetStringField("group"); //Присвоение переменным данных из JSON
//
//
//	}
//}
//


