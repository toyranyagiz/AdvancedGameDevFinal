#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeyEscapeHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API UKeyEscapeHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void ShowInteractionPrompt(const FText& PromptText);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void HideInteractionPrompt();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateStatusBars(
        float HealthPercent,
        float BatteryPercent,
        const FText& ItemTextValue,
        const FText& AmmoTextValue
    );

protected:
    virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* InteractionText;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* BatteryBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* AmmoText;
};