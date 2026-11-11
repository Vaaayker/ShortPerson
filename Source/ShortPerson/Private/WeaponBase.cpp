// WeaponBase.cpp
#include "WeaponBase.h"
#include "ProjectileBase.h"

AWeaponBase::AWeaponBase()
{
    PrimaryActorTick.bCanEverTick = false;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SetRootComponent(Mesh);
    Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
    Muzzle->SetupAttachment(Mesh);
}

void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();
    CurrentAmmo = MagazineSize;
}

void AWeaponBase::StartFire()
{
    const float Interval = 1.f / FireRate;
    const float Delay = FMath::Max(0.f, LastFireTime + Interval - GetWorld()->GetTimeSeconds());

    if (Delay <= 0.f) Fire();
    if (bAutomatic)
        GetWorldTimerManager().SetTimer(FireTimer, this, &AWeaponBase::Fire, Interval, true, Delay > 0.f ? Delay : Interval);
}

void AWeaponBase::StopFire()
{
    GetWorldTimerManager().ClearTimer(FireTimer);
}

void AWeaponBase::Reload()
{
    CurrentAmmo = MagazineSize;
    OnAmmoChanged.Broadcast(CurrentAmmo, MagazineSize);
}

void AWeaponBase::Fire()
{
    if (CurrentAmmo <= 0) { StopFire(); OnEmpty(); return; }
    if (!ProjectileClass) return;

    LastFireTime = GetWorld()->GetTimeSeconds();
    --CurrentAmmo;

    const FVector MuzzleLoc = Muzzle->GetComponentLocation();
    const FVector AimDir = (GetAimPoint() - MuzzleLoc).GetSafeNormal();
    const float HalfAngleRad = PelletsPerShot > 1 ? FMath::DegreesToRadians(SpreadAngle) : 0.f;

    FActorSpawnParameters Params;
    Params.Owner = this;
    Params.Instigator = GetInstigator();
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (int32 i = 0; i < PelletsPerShot; ++i)
    {
        const FVector Dir = HalfAngleRad > 0.f ? FMath::VRandCone(AimDir, HalfAngleRad) : AimDir;
        GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, MuzzleLoc, Dir.Rotation(), Params);
    }

    OnAmmoChanged.Broadcast(CurrentAmmo, MagazineSize);
    OnFired();
}

FVector AWeaponBase::GetAimPoint() const
{
    const APawn* Shooter = GetInstigator();
    if (!Shooter || !Shooter->GetController())
        return Muzzle->GetComponentLocation() + GetActorForwardVector() * MaxRange;

    FVector ViewLoc; FRotator ViewRot;
    Shooter->GetController()->GetPlayerViewPoint(ViewLoc, ViewRot);
    const FVector End = ViewLoc + ViewRot.Vector() * MaxRange;

    FCollisionQueryParams Q(SCENE_QUERY_STAT(WeaponAim), false, Shooter);
    Q.AddIgnoredActor(this);
    FHitResult Hit;
    return GetWorld()->LineTraceSingleByChannel(Hit, ViewLoc, End, ECC_Visibility, Q) ? Hit.ImpactPoint : End;
}

