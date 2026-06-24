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

	UFUNCTION(BlueprintCallable)
	void UpdateSpeed();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool BIsInCombat{ false };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentSpeed{ 0.0f };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentDirection{ 0.0f };

public:
	UFUNCTION(BlueprintCallable)
	void HandleUpdatedTarget(AActor* NewTargetActorRef);

	UFUNCTION(BlueprintCallable)
	void UpdateDirection();
};

