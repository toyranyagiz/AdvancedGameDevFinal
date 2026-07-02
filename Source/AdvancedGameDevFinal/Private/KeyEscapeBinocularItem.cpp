#include "KeyEscapeBinocularItem.h"

AKeyEscapeBinocularItem::AKeyEscapeBinocularItem()
{
    ItemName = FText::FromString("Binocular");

    HandLocation = FVector(35.0f, 10.0f, -10.0f);
    HandRotation = FRotator(0.0f, 0.0f, 0.0f);
    HandScale = FVector(0.25f, 0.25f, 0.25f);
}