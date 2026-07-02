#include "KeyEscapeHUDWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UKeyEscapeHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    HideInteractionPrompt();

    if (HealthBar)
    {
        HealthBar->SetPercent(1.0f);
    }

    if (BatteryBar)
    {
        BatteryBar->SetPercent(0.0f);
    }
}

void UKeyEscapeHUDWidget::ShowInteractionPrompt(const FText& PromptText)
{
    if (InteractionText)
    {
        InteractionText->SetText(PromptText);
        InteractionText->SetVisibility(ESlateVisibility::Visible);
    }
}

void UKeyEscapeHUDWidget::HideInteractionPrompt()
{
    if (InteractionText)
    {
        InteractionText->SetText(FText::GetEmpty());
        InteractionText->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UKeyEscapeHUDWidget::UpdateStatusBars(
    float HealthPercent,
    float BatteryPercent,
    const FText& ItemTextValue,
    const FText& AmmoTextValue
)
{
    if (HealthBar)
    {
        HealthBar->SetPercent(FMath::Clamp(HealthPercent, 0.0f, 1.0f));
    }

    if (BatteryBar)
    {
        BatteryBar->SetPercent(FMath::Clamp(BatteryPercent, 0.0f, 1.0f));
    }

    if (ItemText)
    {
        ItemText->SetText(ItemTextValue);
    }

    if (AmmoText)
    {
        AmmoText->SetText(AmmoTextValue);
    }
}