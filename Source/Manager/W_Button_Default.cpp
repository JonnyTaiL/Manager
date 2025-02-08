// Fill out your copyright notice in the Description page of Project Settings.


#include "W_Button_Default.h"
#include "Engine.h"

void UW_Button_Default::NativeConstruct()
{
	OnDefaultButtonClicked.AddDynamic(this, &UW_Button_Default::Event_OnButtonClicked_Implementation);

}

void UW_Button_Default::Event_OnButtonClicked_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ButtonClicked_CPP"));
}
