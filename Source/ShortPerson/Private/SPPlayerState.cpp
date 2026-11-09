#include "SPPlayerState.h"
#include "Net/UnrealNetwork.h"

void ASPPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPPlayerState, CoinsCollected);
	DOREPLIFETIME(ASPPlayerState, Deaths);
}

void ASPPlayerState::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ASPPlayerState::BeginPlay"));
}

