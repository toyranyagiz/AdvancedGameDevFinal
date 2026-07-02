#include "KeyEscapePistolItem.h"

#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "KeyEscapeCharacter.h"
#include "Kismet/GameplayStatics.h"

AKeyEscapePistolItem::AKeyEscapePistolItem()
{
    ItemName = FText::FromString("Pistol");

    HandLocation = FVector(38.0f, 14.0f, -14.0f);
    HandRotation = FRotator(0.0f, 0.0f, 0.0f);
    HandScale = FVector(0.22f, 0.22f, 0.22f);

    Damage = 35.0f;
    Range = 2500.0f;
    FireCooldown = 0.25f;

    MagazineSize = 7;
    CurrentAmmo = 7;
    ReserveAmmo = 14;

    FireSound = nullptr;
    EmptySound = nullptr;
    ReloadSound = nullptr;

    bCanFire = true;
}

void AKeyEscapePistolItem::UseItem(AKeyEscapeCharacter* UserCharacter)
{
    if (!UserCharacter)
    {
        return;
    }

    if (CurrentAmmo <= 0)
    {
        if (EmptySound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                EmptySound,
                UserCharacter->GetActorLocation()
            );
        }
        return;
    }

    UCameraComponent* PlayerCamera = UserCharacter->GetFirstPersonCamera();

    if (!PlayerCamera)
    {
        return;
    }

    CurrentAmmo--;

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            FireSound,
            UserCharacter->GetActorLocation()
        );
    }

    FVector StartLocation = PlayerCamera->GetComponentLocation();

    FVector EndLocation =
        StartLocation +
        PlayerCamera->GetForwardVector() * Range;

    FHitResult HitResult;

    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(UserCharacter);
    TraceParams.AddIgnoredActor(this);
    TraceParams.bTraceComplex = false;

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility,
        TraceParams
    );

    if (bHit && HitResult.GetActor())
    {
        AActor* HitActor = HitResult.GetActor();

        UGameplayStatics::ApplyPointDamage(
            HitActor,
            Damage,
            PlayerCamera->GetForwardVector(),
            HitResult,
            UserCharacter->GetController(),
            this,
            nullptr
        );
    }
    else
    {
     
    }
}

void AKeyEscapePistolItem::ReloadItem(AKeyEscapeCharacter* UserCharacter)
{
    if (!UserCharacter)
    {
        return;
    }

    if (CurrentAmmo >= MagazineSize)
    {
        return;
    }

    if (ReserveAmmo <= 0)
    {
        return;
    }

    int32 NeededAmmo = MagazineSize - CurrentAmmo;
    int32 AmmoToReload = FMath::Min(NeededAmmo, ReserveAmmo);

    CurrentAmmo += AmmoToReload;
    ReserveAmmo -= AmmoToReload;

    if (ReloadSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            ReloadSound,
            UserCharacter->GetActorLocation()
        );
    }
}

void AKeyEscapePistolItem::AddReserveAmmo(int32 AmmoAmount)
{
    if (AmmoAmount <= 0)
    {
        return;
    }

    ReserveAmmo += AmmoAmount;
}

void AKeyEscapePistolItem::ResetFire()
{
    bCanFire = true;
}

int32 AKeyEscapePistolItem::GetCurrentAmmo() const
{
    return CurrentAmmo;
}

int32 AKeyEscapePistolItem::GetReserveAmmo() const
{
    return ReserveAmmo;
}