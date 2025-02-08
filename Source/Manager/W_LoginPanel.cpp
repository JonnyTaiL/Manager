// Fill out your copyright notice in the Description page of Project Settings.


#include "W_LoginPanel.h"
#include "Engine.h"

void UW_LoginPanel::NativeConstruct()
{
	BTN_Enter->OnDefaultButtonClicked.AddDynamic(this, &UW_LoginPanel::OnEnterButtonClicked);
	BTN_LoginRegistration->OnDefaultButtonClicked.AddDynamic(this, &UW_LoginPanel::OnLoginRegistrationButtonClicked);
	BTN_Register->OnDefaultButtonClicked.AddDynamic(this, &UW_LoginPanel::OnRegisterButtonClicked);
}

void UW_LoginPanel::OnEnterButtonClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("LoginButtonClicked_CPP"));

	//Логика на нажатие кнопки входа/авторизации



}

void UW_LoginPanel::OnLoginRegistrationButtonClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("LoginRegistrationButtonClicked_CPP"));

	//Логика на нажатие кнопки перехода между авторизацией и регистрацией





}

void UW_LoginPanel::OnRegisterButtonClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("RegistrationButtonClicked_CPP"));

	//Логика на нажатие кнопки регистрации





}
