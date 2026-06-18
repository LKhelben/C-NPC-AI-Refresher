// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimation.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5CLASS_API UPlayerAnimation : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentVelocity{ 0.0f };

	UFUNCTION(BlueprintCallable)
	void UpdateVelocity();
};
