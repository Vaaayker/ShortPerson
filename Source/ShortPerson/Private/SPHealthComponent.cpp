#include "SPHealthComponent.h"

USPHealthComponent::USPHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;   // Tick не потрібен
}

void USPHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	bIsDead = false;

	if (bDebugKillOnBeginPlay)
	{
		ApplyDamage(1000.f);
	}
}

void USPHealthComponent::ApplyDamage(float Damage)
{
	if (bIsDead || Damage <= 0.f) return;   // не можна вмерти двічі

	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);

	UE_LOG(LogTemp, Log, TEXT("%s took %.0f damage, health = %.0f"),
		*GetNameSafe(GetOwner()), Damage, Health);

	if (Health <= 0.f)
	{
		bIsDead = true;
		OnDeath.Broadcast(GetOwner());
	}
}

