// WeaponBase.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class AProjectileBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, int32, CurrentAmmo, int32, MagazineSize);

UCLASS(Abstract, Blueprintable)
class SHORTPERSON_API AWeaponBase : public AActor
{
    GENERATED_BODY()
public:
    AWeaponBase();

    void StartFire();
    void StopFire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void Reload();

    UFUNCTION(BlueprintPure, Category = "Weapon") int32 GetCurrentAmmo() const { return CurrentAmmo; }
    UFUNCTION(BlueprintPure, Category = "Weapon") int32 GetMagazineSize() const { return MagazineSize; }
    UFUNCTION(BlueprintPure, Category = "Weapon") FText GetWeaponName() const { return WeaponName; }

    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FOnAmmoChanged OnAmmoChanged;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<USceneComponent> Muzzle;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FText WeaponName;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon|Ammo", meta = (ClampMin = "1", UIMax = "100"))
    int32 MagazineSize = 12;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon|Fire")
    bool bAutomatic = false;

    // Пострілів за секунду
    UPROPERTY(EditDefaultsOnly, Category = "Weapon|Fire", meta = (ClampMin = "0.1", ClampMax = "30"))
    float FireRate = 4.f;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon|Fire", meta = (ClampMin = "1", ClampMax = "20"))
    int32 PelletsPerShot = 1;

    // Половина кута конуса розльоту; має сенс тільки для дробу
    UPROPERTY(EditDefaultsOnly, Category = "Weapon|Fire",
        meta = (EditCondition = "PelletsPerShot > 1", EditConditionHides, ClampMin = "0", ClampMax = "30", Units = "Degrees"))
    float SpreadAngle = 8.f;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon|Fire", meta = (ClampMin = "100", Units = "cm"))
    float MaxRange = 10000.f;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon|Projectile")
    TSubclassOf<AProjectileBase> ProjectileClass;

    UFUNCTION(BlueprintImplementableEvent, Category = "Weapon|Events") void OnFired();
    UFUNCTION(BlueprintImplementableEvent, Category = "Weapon|Events") void OnEmpty();

private:
    void Fire();
    FVector GetAimPoint() const;

    int32 CurrentAmmo = 0;
    float LastFireTime = -1000.f;
    FTimerHandle FireTimer;
};