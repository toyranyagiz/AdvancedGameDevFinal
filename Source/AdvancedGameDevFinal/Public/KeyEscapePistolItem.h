#pragma once

#include "CoreMinimal.h"
#include "KeyEscapeItemBase.h"
#include "TimerManager.h"
#include "KeyEscapePistolItem.generated.h"

class USoundBase;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API AKeyEscapePistolItem : public AKeyEscapeItemBase
{
    GENERATED_BODY()

public:
    AKeyEscapePistolItem();

public:
    virtual void UseItem(AKeyEscapeCharacter* UserCharacter) override;

    virtual void ReloadItem(AKeyEscapeCharacter* UserCharacter) override;

    void AddReserveAmmo(int32 AmmoAmount);

    int32 GetCurrentAmmo() const;

    int32 GetReserveAmmo() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Range;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float FireCooldown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 MagazineSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 CurrentAmmo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 ReserveAmmo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* FireSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* EmptySound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* ReloadSound;

    bool bCanFire;

    FTimerHandle FireCooldownTimerHandle;

private:
    void ResetFire();
};