#include "ShortPersonGameMode.h"
#include "SPCoin.h"
#include "SPGameState.h"
#include "SPPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"

AShortPersonGameMode::AShortPersonGameMode()
{
	GameStateClass = ASPGameState::StaticClass();
}

void AShortPersonGameMode::BeginPlay()
{
	Super::BeginPlay();
	StartRound();
}

void AShortPersonGameMode::StartRound()
{
	if (!CoinClass || !GoldCoinClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartRound: CoinClass/GoldCoinClass не задані в BP"));
		return;
	}

	TArray<AActor*> Points;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), Points);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	int32 Spawned = 0;
	for (AActor* Point : Points)
	{
		const FTransform Transform = Point->GetActorTransform();

		if (FMath::FRand() < GoldCoinChance)
		{
			ASPCoin* Coin = GetWorld()->SpawnActorDeferred<ASPCoin>(
				GoldCoinClass, Transform, nullptr, nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			if (Coin)
			{
				Coin->Value = 5;
				UGameplayStatics::FinishSpawningActor(Coin, Transform);
				++Spawned;
			}
		}
		else
		{
			if (GetWorld()->SpawnActor<ASPCoin>(CoinClass, Transform, Params))
			{
				++Spawned;
			}
		}
	}

	ASPGameState* GS = GetGameState<ASPGameState>();
	if (!GS) return;

	GS->CoinsLeft = Spawned;
	GS->RemainingTime = RoundDuration;
	GS->bRoundActive = true;

	GetWorldTimerManager().SetTimer(
		RoundTimerHandle, this, &AShortPersonGameMode::TickRound, 1.f, true);

	UE_LOG(LogTemp, Warning, TEXT("Round started: %d coins, %d s"), Spawned, RoundDuration);
}

void AShortPersonGameMode::TickRound()
{
	ASPGameState* GS = GetGameState<ASPGameState>();
	if (!GS || !GS->bRoundActive) return;

	GS->RemainingTime -= 1;
	UE_LOG(LogTemp, Log, TEXT("Time left: %d"), static_cast<int32>(GS->RemainingTime));

	if (GS->RemainingTime <= 0)
	{
		EndRound(TEXT("time is up"));
	}
}

void AShortPersonGameMode::OnCoinCollected(ASPCoin* Coin)
{
	ASPGameState* GS = GetGameState<ASPGameState>();
	if (!GS || !GS->bRoundActive) return;

	GS->CoinsLeft = FMath::Max(0, GS->CoinsLeft - 1);

	if (GS->CoinsLeft == 0)
	{
		EndRound(TEXT("all coins collected"));
	}
}

void AShortPersonGameMode::OnPlayerDied(AController* Controller)
{
	if (!Controller) return;

	// 1. Лічильник смертей у PlayerState (він переживає смерть pawn)
	if (ASPPlayerState* PS = Controller->GetPlayerState<ASPPlayerState>())
	{
		PS->Deaths++;
		UE_LOG(LogTemp, Warning, TEXT("%s died, deaths = %d"), *PS->GetPlayerName(), PS->Deaths);
	}

	// 2. Старий pawn: від'єднати від контролера і прибрати через 3 с
	if (APawn* Pawn = Controller->GetPawn())
	{
		Pawn->DetachFromControllerPendingDestroy();
		Pawn->SetLifeSpan(3.f);
	}

	// 3. Таймер респавну з параметром
	FTimerHandle RespawnHandle;
	FTimerDelegate RespawnDelegate =
		FTimerDelegate::CreateUObject(this, &AShortPersonGameMode::RespawnPlayer, Controller);
	GetWorldTimerManager().SetTimer(RespawnHandle, RespawnDelegate, RespawnDelay, false);
}

void AShortPersonGameMode::RespawnPlayer(AController* Controller)
{
	ASPGameState* GS = GetGameState<ASPGameState>();
	if (!GS || !GS->bRoundActive) return;   // раунд закінчився, не респавнимо
	if (!IsValid(Controller)) return;        // гравець міг вийти

	RestartPlayer(Controller);
	UE_LOG(LogTemp, Warning, TEXT("Player respawned"));
}

void AShortPersonGameMode::EndRound(const TCHAR* Reason)
{
	ASPGameState* GS = GetGameState<ASPGameState>();
	if (!GS || !GS->bRoundActive) return;   // захист від подвійного виклику

	GS->bRoundActive = false;
	GetWorldTimerManager().ClearTimer(RoundTimerHandle);

	UE_LOG(LogTemp, Warning, TEXT("Round ended (%s)"), Reason);

	for (APlayerState* PSBase : GS->PlayerArray)
	{
		if (ASPPlayerState* PS = Cast<ASPPlayerState>(PSBase))
		{
			UE_LOG(LogTemp, Warning, TEXT("  %s: score %.0f, coins %d, deaths %d"),
				*PS->GetPlayerName(), static_cast<float>(PS->GetScore()),
				static_cast<int32>(PS->CoinsCollected), PS->Deaths);
		}
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			PC->GetPawn()->DisableInput(PC);
		}
	}
}