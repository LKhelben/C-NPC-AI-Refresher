// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NameGeneratorSubsystem.generated.h"

UCLASS()
class UNREAL5CLASS_API UNameGeneratorSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category = "NameGen")
    FString GenerateRandomName(bool bIsMale);

    UFUNCTION(BlueprintCallable, Category = "NameGen")
    void ReloadNameList();

private:
    void LoadNameList();

    TArray<FString> MaleFirstNames;
    TArray<FString> FemaleFirstNames;
    TArray<FString> LastNames;

    FString GetNameListPath() const;
};