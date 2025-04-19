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
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetAllUsVariantsReceive);
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
