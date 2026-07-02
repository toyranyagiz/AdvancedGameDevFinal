#include "KeyEscapeFoodItem.h"

#include "Engine/Engine.h"
#include "KeyEscapeCharacter.h"
#include "Kismet/GameplayStatics.h"

AKeyEscapeFoodItem::AKeyEscapeFoodItem()
{
    HealAmount = 35.0f;

    ItemName = FText::FromString("Food");

    HandLocation = FVector(40.0f, 10.0f, -15.0f);
    HandRotation = FRotator(0.0f, 0.0f, 0.0f);
    HandScale = FVector(0.2f, 0.2f, 0.2f);

    EatSound = nullptr;

}

void AKeyEscapeFoodItem::UseItem(AKeyEscapeCharacter* UserCharacter)
{
    if (!UserCharacter)
    {
        return;
    }

    UserCharacter->HealPlayer(HealAmount);

    if (EatSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            EatSound,
            UserCharacter->GetActorLocation()
        );
    }

    UserCharacter->ConsumeCurrentRightHandItem();
}