#include "iCTFPlusGameMode.h"

#include "EngineGlobals.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

AiCTFPlusGameMode::AiCTFPlusGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

float AiCTFPlusGameMode::AdjustNearbyPlayerStartScore(const AController* Player, const AController* OtherController, const ACharacter* OtherCharacter, const FVector& StartLoc, const APlayerStart* P)
{
	float ScoreAdjust = 0.f;
	float NextDist = (OtherCharacter->GetActorLocation() - StartLoc).Size();
	bool bTwoPlayerGame = (NumPlayers + NumBots == 2);
	if (((NextDist < 5000.0f) || bTwoPlayerGame) && !UTGameState->OnSameTeam(Player, OtherController))
	{
		static FName NAME_RatePlayerStart = FName(TEXT("RatePlayerStart"));
		bool bIsLastKiller = (OtherCharacter->PlayerState == Cast<AUTPlayerState>(Player->PlayerState)->LastKillerPlayerState);
		if (!GetWorld()->LineTraceTestByChannel(StartLoc, OtherCharacter->GetActorLocation() + FVector(0.f, 0.f, OtherCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), COLLISION_TRACE_WEAPONNOCHARACTER, FCollisionQueryParams(NAME_RatePlayerStart, false)))
		{
			// Avoid the last person that killed me
			if (bIsLastKiller)
			{
				ScoreAdjust -= 7.f;
			}

			ScoreAdjust -= (5.f - 0.0008f * NextDist);
		}
		//UE_LOG(LogTemp, Warning, TEXT("Spawn score adjusted due to nearby player. New score adjustment: %f"), ScoreAdjust);
	}

	return ScoreAdjust;
}
