// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerHUD.h"
//#include "ManagerTypes.h"

void AManagerHUD::NativeConstruct()
{

}

//void AManagerHUD::SetUserData(FString m_Login, FString m_Name, FString m_Surname, FString m_Patronumic, EPermissionType m_PermissionType, FString m_GroupName)
//{
//	Login = m_Login;
//	Name = m_Name;
//	Surname = m_Surname;
//	Patronumic = m_Patronumic;
//	PermissionType = m_PermissionType;
//	GroupName = m_GroupName;
//}


FManagerUserData AManagerHUD::GetUserData_Implementation()
{
	FManagerUserData UserData;
	UserData.Login = Login;
	UserData.Name = Name;
	UserData.Surname = Surname;
	UserData.Patronumic = Patronumic;
	UserData.GroupName = GroupName;
	UserData.PermissionType = PermissionType;

	return UserData;
}

void AManagerHUD::SetUserData_Implementation(FString& m_Login, FString& m_Name, FString& m_Surname, FString& m_Patronumic, EPermissionType& m_PermissionType, FString& m_GroupName)
{
	Login = m_Login;
	Name = m_Name;
	Surname = m_Surname;
	Patronumic = m_Patronumic;
	PermissionType = m_PermissionType;
	GroupName = m_GroupName;
}
