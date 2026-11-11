// ProjectileBase.cpp
#include "ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectileBase::AProjectileBase()
{
    PrimaryActorTick.bCanEverTick = false;
    InitialLifeSpan = 3.f;

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->InitSphereRadius(5.f);
    Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    Collision->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
    SetRootComponent(Collision);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(Collision);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    Movement->UpdatedComponent = Collision;
    Movement->InitialSpeed = 4000.f;
    Movement->MaxSpeed = 4000.f;
    Movement->bRotationFollowsVelocity = true;
    Movement->ProjectileGravityScale = 0.f;
}

void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();
    if (APawn* Shooter = GetInstigator())
        Collision->IgnoreActorWhenMoving(Shooter, true); // не влучати в себе
}

void AProjectileBase::OnHit(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    FVector, const FHitResult& Hit)
{
    const FVector Dir = Movement->Velocity.GetSafeNormal();
    if (OtherActor && OtherActor != this)
    {
        if (OtherComp && OtherComp->IsSimulatingPhysics())
            OtherComp->AddImpulseAtLocation(Dir * ImpactVelocity * OtherComp->GetMass(), Hit.ImpactPoint);

        UGameplayStatics::ApplyPointDamage(OtherActor, Damage, Dir, Hit, GetInstigatorController(), this, nullptr);
    }
    OnImpact(Hit); // хук для BP: звук, партикли
    Destroy();
}