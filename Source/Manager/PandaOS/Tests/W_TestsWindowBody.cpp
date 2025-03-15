// Fill out your copyright notice in the Description page of Project Settings.



#include "W_TestsWindowBody.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/Interfaces/IHUDRequestData.h"
#include "Manager/ManagerHUD.h"
#include "Json.h"
#include "Engine.h"

void UW_TestsWindowBody::NativeConstruct()
{

	//GetVariants
	GetVariantsDataSend();


}

void UW_TestsWindowBody::GetVariantsDataSend()
{

	AHUD* Hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD();
	AManagerHUD* MHud = Cast<AManagerHUD>(Hud);
	FString group = MHud->GroupName;

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
	Request->SetContentAsString(OutputString);
	Request->SetURL(URL);
	Request->ProcessRequest();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, group);

}

void UW_TestsWindowBody::GetVariantsDataReceive(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//TArray<FString> StringArray;

	TestsArrayIds.Empty();

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	FString answer = Response->GetContentAsString();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, answer);

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //������� JSON ������ �������
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	FJsonSerializer::Deserialize(JsonReader, JsonArray);

	for (const TSharedPtr<FJsonValue>& JsonValue : JsonArray)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, answer);

			TestsArrayIds.Add(JsonValue->AsString());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, JsonValue->AsString());
		
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
		//TestDataArray[counter].QuestionText = Question; // НУ ТУТ ВООБЩЕ ВСЯ ХУЙНЯ В ОДНУ ПЕРЕМЕННУЮ ПЕРЕЗАПИСЫВАЕТСЯ И ХУЙНЯЧ НА ВЫХОДЕ, ПОТОМ ПОЧИНИШЬ
		//TestDataArray[counter].QuestionType = QuestionType;
		//TestDataArray[counter].Answer_1 = Answer_1;
		//TestDataArray[counter].Answer_2 = Answer_2;
		//TestDataArray[counter].Answer_3 = Answer_3;
		//TestDataArray[counter].Answer_4 = Answer_4;
		//TestDataArray[counter].CorrectAnswer = Correct_Answer;


		UE_LOG(LogTemp, Warning, TEXT("%s"), *Question);
		UE_LOG(LogTemp, Warning, TEXT("%d"), QuestionType);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Answer_1);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Answer_2);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Answer_3);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Answer_4);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Correct_Answer);

		counter++;
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
	//TArray<FString> StringArray;

	CompletedTestsArrayIds.Empty();

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	FString answer = Response->GetContentAsString();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, answer);

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(answer); //������� JSON ������ �������
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	FJsonSerializer::Deserialize(JsonReader, JsonArray);

	for (const TSharedPtr<FJsonValue>& JsonValue : JsonArray)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, answer);

		CompletedTestsArrayIds.Add(JsonValue->AsString());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, JsonValue->AsString());

	}
}


void UW_TestsWindowBody::QuestionSend(FString UserAnswer, FString CorrectAnswer)
{
	FString OutputString;
	FString Content = "You have been given the perfect answer to the question and the answer given by the student. Evaluate on a scale from 0 to 5 points how much the student's answer matches the meaning of the ideal answer. The ideal answer:" + CorrectAnswer + "." + "The student's answer: " + UserAnswer + ". Give the answer in the form of a character - from 0 to 5.";
	//TB_Answer->SetText(FText::FromString("Wait"));
	FString URL = "http://26.76.184.253:1234/v1/chat/completions";

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("model", "gemma-3-4b-it");

	// Ð¡Ð¾Ð·Ð´Ð°ÐµÐ¼ Ð¼Ð°ÑÑÐ¸Ð² ÑÐ¾Ð¾Ð±Ñ‰ÐµÐ½Ð¸Ð¹
	TArray<TSharedPtr<FJsonValue>> MessagesArray;
	// Ð¡Ð¾Ð·Ð´Ð°ÐµÐ¼ ÑÐ¾Ð¾Ð±Ñ‰ÐµÐ½Ð¸Ñ
	TSharedPtr<FJsonObject> UserMessage = MakeShareable(new FJsonObject());
	UserMessage->SetStringField("role", "user");
	UserMessage->SetStringField("content", Content);
	MessagesArray.Add(MakeShareable(new FJsonValueObject(UserMessage)));
	TSharedPtr<FJsonObject> SystemMessage = MakeShareable(new FJsonObject());
	SystemMessage->SetStringField("role", "assistant");
	SystemMessage->SetStringField("content", "You are machine for rating student answers with a number from 0 to 5, you are only allowed to answer with a simple number representing the score you give for the answer");
	MessagesArray.Add(MakeShareable(new FJsonValueObject(SystemMessage)));
	// Ð”Ð¾Ð±Ð°Ð²Ð»ÑÐµÐ¼ Ð¼Ð°ÑÑÐ¸Ð² ÑÐ¾Ð¾Ð±Ñ‰ÐµÐ½Ð¸Ð¹ Ð² JSON Ð¾Ð±ÑŠÐµÐºÑ‚
	JsonObject->SetArrayField("messages", MessagesArray);

	JsonObject->SetNumberField("temperature", 0);
	JsonObject->SetNumberField("max_tokens", -1);
	JsonObject->SetBoolField("stream", false);

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
	FString answer = Response->GetContentAsString();
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(answer);

	FJsonSerializer::Deserialize(JsonReader, JsonObject);
	const TArray<TSharedPtr<FJsonValue>>* ChoicesArray;
	JsonObject->TryGetArrayField(TEXT("choices"), ChoicesArray);
	const TSharedPtr<FJsonValue>& FirstChoiceValue = (*ChoicesArray)[0];
	const TSharedPtr<FJsonObject>* ChoiceObject;
	if (FirstChoiceValue->TryGetObject(ChoiceObject))
	{
		const TSharedPtr<FJsonObject>* MessageObject;
		if ((*ChoiceObject)->TryGetObjectField(TEXT("message"), MessageObject))
		{
			FString Content;
			if ((*MessageObject)->TryGetStringField(TEXT("content"), Content))
			{


				//  TB_Answer->SetText(FText::FromString(""));
				//TB_Answer->SetText(FText::FromString(Content));
				AnswerRating = Content;
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Content);
			   // UE_LOG(LogTemp, Log, TEXT("Content: %s"), *Content);
			}
		}
	}
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, answer);
}