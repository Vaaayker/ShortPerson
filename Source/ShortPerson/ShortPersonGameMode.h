#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShortPersonGameMode.generated.h"

class ASPCoin;

UCLASS(abstract)
class AShortPersonGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AShortPersonGameMode();
	void OnCoinCollected(ASPCoin* Coin);
	void OnPlayerDied(AController* Controller);

protected:
	virtual void BeginPlay() override;

	void StartRound();
	void TickRound();
	void EndRound(const TCHAR* Reason);
	void RespawnPlayer(AController* Controller);

	UPROPERTY(EditDefaultsOnly, Category = "Coins")
	TSubclassOf<ASPCoin> CoinClass;

	UPROPERTY(EditDefaultsOnly, Category = "Coins")
	TSubclassOf<ASPCoin> GoldCoinClass;

	UPROPERTY(EditDefaultsOnly, Category = "Coins", meta = (ClampMin = "0", ClampMax = "1"))
	float GoldCoinChance = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Round", meta = (ClampMin = "1"))
	int32 RoundDuration = 60;

	UPROPERTY(EditDefaultsOnly, Category = "Round", meta = (ClampMin = "0"))
	float RespawnDelay = 3.f;

	FTimerHandle RoundTimerHandle;
};