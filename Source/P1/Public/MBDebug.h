#pragma once
#include <source_location>
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(MB, Log, All);

enum class DebugType
{
	Error,
	Success,
	Normal,
};

namespace DebugUtils
{
	inline static void PrintDebugMessage(const FString& InMessage, DebugType InDebugType, float InDisplayTime = 15.f)
	{
		if (nullptr == GEngine)
			return;

		switch (InDebugType)
		{
		case DebugType::Error:
			UE_LOG(MB, Error, TEXT("%s"), *InMessage);
			GEngine->AddOnScreenDebugMessage(-1, InDisplayTime, FColor::Red, InMessage);
			break;
		case DebugType::Success:
			UE_LOG(MB, Warning, TEXT("%s"), *InMessage);
			GEngine->AddOnScreenDebugMessage(-1, InDisplayTime, FColor::Green, InMessage);
			break;
		case DebugType::Normal:
			UE_LOG(MB, Warning, TEXT("%s"), *InMessage);
			GEngine->AddOnScreenDebugMessage(-1, InDisplayTime, FColor::Cyan, InMessage);
			break;
		default:
			check(nullptr);
			break;
		}
	}

	inline static FString ExtractFileName(const FString& InFilePath)
	{
		size_t Position = InFilePath.Find(TEXT("Private"));
		if (Position != std::string::npos)
		{
			return InFilePath.Mid(Position + FString(TEXT("Private")).Len());
		}

		Position = InFilePath.Find(TEXT("Public"));
		if (Position != std::string::npos)
		{
			return InFilePath.Mid(Position + FString(TEXT("Pulbic")).Len());
		}

		return InFilePath;
	}
}

namespace Debug
{
	inline static void Print(const FString& InMessage, const float InDisplayTime = 15.f)
	{
		DebugUtils::PrintDebugMessage(InMessage, DebugType::Normal, InDisplayTime);
	}

	inline static void Error(const float InDisplayTime = 15.f, const std::source_location InLocation = std::source_location::current())
	{
		FString Message = TEXT("Error => ");
		Message += InLocation.function_name();
		Message += TEXT(" = ");
		Message += DebugUtils::ExtractFileName(InLocation.file_name());
		Message += TEXT(" = ");
		Message += FString::FromInt(InLocation.line());
		DebugUtils::PrintDebugMessage(Message, DebugType::Error, InDisplayTime);
	}

	inline static void Success(const float InDisplayTime = 15.f, const std::source_location InLocation = std::source_location::current())
	{
		FString Message = TEXT("Success => ");
		Message += InLocation.function_name();
		DebugUtils::PrintDebugMessage(Message, DebugType::Success, InDisplayTime);
	}

	inline static void Called(const float InDisplayTime = 15.f, const std::source_location InLocation = std::source_location::current())
	{
		FString Message = TEXT("Called => ");
		Message += InLocation.function_name();
		DebugUtils::PrintDebugMessage(Message, DebugType::Normal, InDisplayTime);
	}

	template<typename T>
	inline static bool NullCheck(T InObject, const FString& InMessage, float InDisplayTime = 15.f, const std::source_location InLocation = std::source_location::current())
	{
		if (nullptr != InObject)
		{
			FString Message = TEXT("Not Null => ");
			Message += InMessage;
			Message += TEXT(" = ");
			Message += InLocation.function_name();
			DebugUtils::PrintDebugMessage(Message, DebugType::Success, InDisplayTime);

			return true;
		}

		FString Message = TEXT("Null => ");
		Message += InMessage;
		Message += TEXT(" = ");
		Message += InLocation.function_name();
		Message += TEXT(" = ");
		Message += DebugUtils::ExtractFileName(InLocation.file_name());
		Message += TEXT(" = ");
		Message += FString::FromInt(InLocation.line());
		DebugUtils::PrintDebugMessage(Message, DebugType::Error, InDisplayTime);

		return false;
	}

	template<typename T>
	inline static bool NullPrint(T InObject, const FString& InMessage, float InDisplayTime = 15.f, const std::source_location InLocation = std::source_location::current())
	{
		if (nullptr != InObject)
			return true;

		FString Message = TEXT("Null => ");
		Message += InMessage;
		Message += TEXT(" = ");
		Message += InLocation.function_name();
		Message += TEXT(" = ");
		Message += DebugUtils::ExtractFileName(InLocation.file_name());
		Message += TEXT(" = ");
		Message += FString::FromInt(InLocation.line());
		DebugUtils::PrintDebugMessage(Message, DebugType::Error, InDisplayTime);

		return false;
	}
}

namespace DrawDebug
{
	inline static void DrawSphere(const UWorld* InWorld, const FVector& InLocation, FColor InColor = FColor::Black, const float InRadius = 25.f, const int32 InSegments = 4)
	{
		if (nullptr == InWorld)
			return;

		DrawDebugSphere(InWorld, InLocation, InRadius, InSegments, InColor, false, -1.f, 0U, 4.f);
	}

	inline static void DrawLine(const UWorld* InWorld, const FVector& InStartLocation, const FVector& InEndLocation, FColor InColor = FColor::Black)
	{
		if (nullptr == InWorld)
			return;

		DrawDebugLine(InWorld, InStartLocation, InEndLocation, InColor, false, -1.f, 0U, 3.f);
	}

	inline static void DrawString(const UWorld* InWorld, const FVector& InLocation, const FString& InMessage)
	{
		if (nullptr == InWorld)
			return;

		DrawDebugString(InWorld, InLocation, InMessage, 0, FColor::White, -1.f, false);
	}
}