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

USTRUCT(BlueprintType)
struct FGeneratedTestStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 QuestionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString QuestionText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CorrectAnswer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Answer_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Answer_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Answer_4;





};


USTRUCT(BlueprintType)
struct FModifierData
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float USCompleteChance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSPModificator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHoursModificator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxTasksModificator;


};


UENUM(BlueprintType)
enum class EProficiencies : uint8
{
	Modeling = 0 UMETA(DisplayName = "Modeling"),
	Art = 1 UMETA(DisplayName = "Art"),
	Code = 2 UMETA(DisplayName = "Code")

};


USTRUCT(BlueprintType)
struct FProficiency
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Modeling = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Art = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Code = 0.0f;

};

USTRUCT(BlueprintType)
struct FProficiencyRequare
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Modeling = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Art = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Code = false;

};



USTRUCT(BlueprintType)
struct FEmployeeData
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHours;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxTasks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Skill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FProficiency Proficiency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FModifierData> Buffs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FModifierData> Debuffs;


};


USTRUCT(BlueprintType)
struct FUSData
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Complexity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Hours;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> DoBefore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> ChildUS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> ParentUS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FProficiencyRequare Proficiencies;

};

USTRUCT(BlueprintType)
struct FSimVariantData
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Days;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Sprints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Group_ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEmployeeData> Employees;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FUSData> UserStories;

};

