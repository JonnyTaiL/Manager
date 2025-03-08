// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManagerTypes.generated.h"



/**
 * 
 */
class MANAGER_API ManagerTypes
{
public:
	ManagerTypes();
	~ManagerTypes();






};


UENUM(BlueprintType)
enum class EPermissionType : uint8
{
	Student,
	Teacher,
	Admin,
};


USTRUCT(BlueprintType)
struct FManagerUserData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UserID = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Login = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Surname = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Patronumic = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPermissionType PermissionType = EPermissionType::Student;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GroupName = "";
};



USTRUCT(BlueprintType)
struct FTestDataArrayStruct
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString QuestionText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 QuestionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Answer_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Answer_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Answer_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Answer_4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CorrectAnswer;




};
