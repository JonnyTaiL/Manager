// Fill out your copyright notice in the Description page of Project Settings.


#include "W_Textbox_Clickable.h"
//#include "Components/Button.h"

void UW_Textbox_Clickable::NativeConstruct()
{
	OnDefaultButtonClicked.AddDynamic(this, &UW_Textbox_Clickable::Event_OnButtonClicked_Implementation);
}

void UW_Textbox_Clickable::Event_OnButtonClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ButtonClicked_CPP"));
}


