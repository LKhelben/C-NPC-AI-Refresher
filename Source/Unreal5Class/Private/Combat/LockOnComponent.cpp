// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/LockOnComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/Enemy.h"

// Sets default values for this component's properties
ULockOnComponent::ULockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerRef = GetOwner<ACharacter>();
	Controller = GetWorld()->GetFirstPlayerController();
	MovementComp = OwnerRef->GetCharacterMovement();
	SpringArmComp = OwnerRef->FindComponentByClass<USpringArmComponent>();
}

void ULockOnComponent::StartLockon(float Radius)
{
    FVector CurrentLocation{ OwnerRef->GetActorLocation() };
    FCollisionShape Sphere{ FCollisionShape::MakeSphere(Radius) };
    FCollisionQueryParams IgnoreParams{
        FName{ TEXT("Ignore Collision Params") },
        false,
        OwnerRef
    };

    TArray<FOverlapResult> OutResults;
    bool bHasFoundTargets{ GetWorld()->OverlapMultiByChannel(
        OutResults,
        CurrentLocation,
        FQuat::Identity,
        ECollisionChannel::ECC_GameTraceChannel1,
        Sphere,
        IgnoreParams
    ) };

    if (!bHasFoundTargets) { return; }

    FVector LookDirection{ Controller->GetControlRotation().Vector() };

    AActor* BestTarget = nullptr;
    float BestScore = -FLT_MAX;

    // Tune this to bias more toward angle (higher value) or distance (lower value)
    constexpr float DistanceWeight = 1.0f;

    for (const FOverlapResult& Result : OutResults)
    {
        AActor* CandidateActor = Result.GetActor();
        if (!CandidateActor || !CandidateActor->Implements<UEnemy>()) { continue; }

        FVector ToTarget{ CandidateActor->GetActorLocation() - CurrentLocation };
        float Distance{ static_cast<float>(ToTarget.Size()) };
        FVector DirectionToTarget{ ToTarget.GetSafeNormal() };

        float Dot{ static_cast<float>(FVector::DotProduct(LookDirection, DirectionToTarget)) };
        float NormalizedDistance{ Distance / Radius }; // 0.0 (close) to 1.0 (edge of sphere)

        float Score{ Dot - (NormalizedDistance * DistanceWeight) };

        if (Score > BestScore)
        {
            BestScore = Score;
            BestTarget = CandidateActor;
        }
    }

    if (!BestTarget) { return; }

    CurrentTargetActor = BestTarget;
    Controller->SetIgnoreLookInput(true);
    MovementComp->bOrientRotationToMovement = false;
    MovementComp->bUseControllerDesiredRotation = true;
    SpringArmComp->TargetOffset = FVector{ 0.0, 0.0, 100.0 };
    IEnemy::Execute_OnSelect(CurrentTargetActor);
    OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
}

void ULockOnComponent::EndLockon()
{
	IEnemy::Execute_OnDeselect(CurrentTargetActor);
	CurrentTargetActor = nullptr;
	MovementComp->bOrientRotationToMovement = true;
	MovementComp->bUseControllerDesiredRotation = false;
	SpringArmComp->TargetOffset = FVector::ZeroVector;

	Controller->ResetIgnoreLookInput();

	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
}

void ULockOnComponent::ToggleLockon(float Radius)
{
	if (IsValid(CurrentTargetActor)) {
		EndLockon();
	}
	else {
		StartLockon(Radius);
	}
}


// Called every frame
void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(CurrentTargetActor)) { return; }
	FVector CurrentLocation{ OwnerRef->GetActorLocation() };
	FVector TargetLocation{ CurrentTargetActor->GetActorLocation() };

	double TargetDistance{
		FVector::Distance(CurrentLocation, TargetLocation)
	};

	if (TargetDistance >= BreakDistance) {
		EndLockon();
		return;
	}

	TargetLocation.Z -= 125;


	FRotator NewRotation{ UKismetMathLibrary::FindLookAtRotation(
		CurrentLocation, TargetLocation
	) };

	Controller->SetControlRotation(NewRotation);
}

