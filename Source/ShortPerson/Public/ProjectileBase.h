// ProjectileBase.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(Abstract, Blueprintable)
class SHORTPERSON_API AProjectileBase : public AActor
{
    GENERATED_BODY()
public:
    AProjectileBase();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
    TObjectPtr<USphereComponent> Collision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
    TObjectPtr<UProjectileMovementComponent> Movement;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Damage", meta = (ClampMin = "0"))
    float Damage = 20.f;

    // Зміна швидкості фізичного об'єкта при влучанні, см/с
    UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage", meta = (ClampMin = "0"))
    float ImpactVelocity = 300.f;

    UFUNCTION(BlueprintImplementableEvent, Category = "Projectile")
    void OnImpact(const FHitResult& Hit);

private:
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        FVector NormalImpulse, const FHitResult& Hit);
};
