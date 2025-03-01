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
	//if (Hud->GetClass()->ImplementsInterface(UIHUDRequestData::StaticClass()))
	//{
	//	IIHUDRequestData::;
	//}

	AManagerHUD* MHud = Cast<AManagerHUD>(Hud);
	FString group = MHud->GroupName;



	FString URL = "http://26.76.184.253:8000/getvariantsdata";
	URL = URL + "?group=" + group;
	FString OutputString;



	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetVariantsDataReceive);
	Request->SetVerb("GET");
	Request->SetURL(URL);
	Request->ProcessRequest();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, group);


	/*
	FString URL = "http://26.76.184.253:8000/getvariantsdata";
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("group", group);



	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::GetVariantsDataReceive);
	Request->SetVerb("POST");
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(OutputString);
	Request->ProcessRequest();
*/

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, group);

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
