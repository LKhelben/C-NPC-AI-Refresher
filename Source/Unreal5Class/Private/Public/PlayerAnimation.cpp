// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/PlayerAnimation.h"

void UPlayerAnimation::UpdateVelocity()
{
	APawn* PawnRef{ TryGetPawnOwner() };
	//Tries to get Pawn
	if (!IsValid(PawnRef)) { return; }
	//If fails to get pawn, returns early

	FVector Velocity{ PawnRef->GetVelocity() };
	CurrentVelocity = static_cast<float>(Velocity.Length());

	//Sets Velocity

}
