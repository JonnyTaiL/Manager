// Fill out your copyright notice in the Description page of Project Settings.


#include "W_OSButtonOptions.h"
#include "Components/TextBlock.h"
#include "W_Button_Default.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/Interfaces/IHUDRequestData.h"
#include "ManagerHUD.h"

void UW_OSButtonOptions::NativeConstruct()
{
	AHUD* Hud = UGameplayStatics::GetPlayerController(this, 0)->GetHUD();
	FManagerUserData UserData = Cast<IIHUDRequestData>(Hud)->GetUserData_Implementation();
	TXT_Name->SetText(FText::FromString(UserData.Name));
	TXT_Group->SetText(FText::FromString(UserData.GroupName));
	TXT_AccessLevel->SetText(UEnum::GetDisplayValueAsText(UserData.PermissionType));
}
