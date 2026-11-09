#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SPGameState.generated.h"

UCLASS()
class SHORTPERSON_API ASPGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Round")
	float RemainingTime = 0.f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Round")
	int32 CoinsLeft = 0;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Round")
	bool bRoundActive = false;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
};