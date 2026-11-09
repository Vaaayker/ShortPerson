#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SPHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHORTPERSON_API USPHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USPHealthComponent();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void ApplyDamage(float Damage);

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const { return MaxHealth; }

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeath OnDeath;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Health", meta = (ClampMin = "1"))
	float MaxHealth = 100.f;

	UPROPERTY(VisibleInstanceOnly, Category = "Health")
	float Health = 0.f;

	UPROPERTY(VisibleInstanceOnly, Category = "Health")
	bool bIsDead = false;

	// Тимчасово для перевірки: якщо true, на старті отримає 1000 шкоди
	UPROPERTY(EditAnywhere, Category = "Health|Debug")
	bool bDebugKillOnBeginPlay = false;
};

