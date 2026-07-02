#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "KeyEscapeExitTrigger.generated.h"

class UBoxComponent;
class UUserWidget;
class USoundBase;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API AKeyEscapeExitTrigger : public AActor
{
    GENERATED_BODY()

public:
    AKeyEscapeExitTrigger();

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* TriggerBox;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exit")
    FName RequiredKeyID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> WinScreenWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* WinMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FName MainMenuLevelName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    float ReturnToMainMenuDelay;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Exit")
    bool bHasTriggered;

    FTimerHandle ReturnToMainMenuTimerHandle;

private:
    UFUNCTION()
    void OnExitOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    void ShowWinScreen(APlayerController* PlayerController);

    void OpenMainMenu();
};