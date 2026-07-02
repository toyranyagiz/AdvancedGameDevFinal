#include "KeyEscapeMolotovItem.h"

#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "KeyEscapeCharacter.h"
#include "KeyEscapeMolotovProjectile.h"
#include "Kismet/GameplayStatics.h"

AKeyEscapeMolotovItem::AKeyEscapeMolotovItem()
{
    ItemName = FText::FromString("Molotov");

    HandLocation = FVector(38.0f, 12.0f, -14.0f);
    HandRotation = FRotator(0.0f, 0.0f, 0.0f);
    HandScale = FVector(0.18f, 0.18f, 0.32f);

    ThrowSpeed = 1200.0f;
    ThrowSound = nullptr;
}

void AKeyEscapeMolotovItem::UseItem(AKeyEscapeCharacter* UserCharacter)
{
    if (!UserCharacter)
    {
        return;
    }

    if (!ProjectileClass)
    {
        return;
    }

    UCameraComponent* PlayerCamera = UserCharacter->GetFirstPersonCamera();

    if (!PlayerCamera)
    {
        return;
    }

    FVector SpawnLocation =
        PlayerCamera->GetComponentLocation() +
        PlayerCamera->GetForwardVector() * 80.0f +
        PlayerCamera->GetRightVector() * 20.0f -
        PlayerCamera->GetUpVector() * 15.0f;

    FRotator SpawnRotation = PlayerCamera->GetComponentRotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = UserCharacter;
    SpawnParams.Instigator = UserCharacter;

    AKeyEscapeMolotovProjectile* Projectile =
        UserCharacter->GetWorld()->SpawnActor<AKeyEscapeMolotovProjectile>(
            ProjectileClass,
            SpawnLocation,
            SpawnRotation,
            SpawnParams
        );

    if (Projectile)
    {
        Projectile->LaunchProjectile(
            PlayerCamera->GetForwardVector(),
            ThrowSpeed
        );

        if (ThrowSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                ThrowSound,
                UserCharacter->GetActorLocation()
            );
        }

        UserCharacter->ConsumeCurrentRightHandItem();
    }
}