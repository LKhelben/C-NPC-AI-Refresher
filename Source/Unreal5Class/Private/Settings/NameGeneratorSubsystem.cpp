// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/NameGeneratorSubsystem.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UNameGeneratorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    LoadNameList();
}

FString UNameGeneratorSubsystem::GetNameListPath() const
{
    return FPaths::ProjectDir() / TEXT("Data/Names.txt");
}

void UNameGeneratorSubsystem::LoadNameList()
{
    MaleFirstNames.Empty();
    FemaleFirstNames.Empty();
    LastNames.Empty();

    TArray<FString> Lines;
    const FString FilePath = GetNameListPath();

    if (!FFileHelper::LoadFileToStringArray(Lines, *FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("NameGenerator: could not load %s"), *FilePath);
        return;
    }

    TArray<FString>* CurrentSection = nullptr;

    for (FString Line : Lines)
    {
        Line.TrimStartAndEndInline();
        if (Line.IsEmpty()) continue;

        if (Line.StartsWith("[") && Line.EndsWith("]"))
        {
            const FString SectionName = Line.Mid(1, Line.Len() - 2);

            if (SectionName == "MaleFirstName") CurrentSection = &MaleFirstNames;
            else if (SectionName == "FemaleFirstName") CurrentSection = &FemaleFirstNames;
            else if (SectionName == "LastName") CurrentSection = &LastNames;
            else CurrentSection = nullptr;

            continue;
        }

        if (CurrentSection)
        {
            CurrentSection->Add(Line);
        }
    }
}

void UNameGeneratorSubsystem::ReloadNameList()
{
    LoadNameList();
}

FString UNameGeneratorSubsystem::GenerateRandomName(bool bIsMale)
{
    const TArray<FString>& FirstNames = bIsMale ? MaleFirstNames : FemaleFirstNames;

    if (FirstNames.Num() == 0 || LastNames.Num() == 0)
    {
        return TEXT("Unnamed");
    }

    FString Result = FirstNames[FMath::RandRange(0, FirstNames.Num() - 1)];
    Result += TEXT(" ") + LastNames[FMath::RandRange(0, LastNames.Num() - 1)];

    return Result;
}