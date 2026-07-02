#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TimerManager.h"
#include "KeyEscapeCharacter.generated.h"


class UCameraComponent;
class USceneComponent;
class UInputMappingContext;
class UInputAction;
class UUserWidget;
class UKeyEscapeHUDWidget;
class AKeyEscapeItemBase;
class AKeyEscapeFlashlight;
class AKeyEscapeBinocularItem;
class UUserWidget;
class AKeyEscapePistolItem;
class AActor;
class USoundBase;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API AKeyEscapeCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AKeyEscapeCharacter();

public:
    virtual void Tick(float DeltaSeconds) override;

    bool AddRightHandItem(AKeyEscapeItemBase* ItemToAdd);

    void SelectRightHandSlot(int32 SlotIndex);

    void DropCurrentRightHandItem();

    void UseCurrentRightHandItem();

    void ConsumeCurrentRightHandItem();

    bool PickupFlashlight(AKeyEscapeFlashlight* FlashlightToPickup);

    void AddFlashlightBattery(float Amount);

    void SetReadingNote(bool bReading);

    bool AddPistolAmmo(int32 AmmoAmount);

    UCameraComponent* GetFirstPersonCamera() const;

    UFUNCTION(BlueprintCallable, Category = "Keys")
    void AddKey(FName KeyID);

    UFUNCTION(BlueprintCallable, Category = "Keys")
    bool HasKey(FName KeyID) const;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void TakePlayerDamage(float DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void HealPlayer(float HealAmount);

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetCurrentHealth() const;

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetMaxHealth() const;

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const;

protected:
    virtual void BeginPlay() override;

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* FirstPersonCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hands")
    USceneComponent* LeftHandPoint;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hands")
    USceneComponent* RightHandPoint;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* SprintAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* InteractAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* SelectSlot1Action;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* SelectSlot2Action;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* SelectSlot3Action;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* SelectSlot4Action;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* DropAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* ToggleFlashlightAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* UseItemAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* ZoomAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* ReloadAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* PauseAction;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeed;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    float InteractionDistance;

    UPROPERTY()
    AActor* CurrentInteractableActor;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UKeyEscapeHUDWidget> HUDWidgetClass;

    UPROPERTY()
    UKeyEscapeHUDWidget* HUDWidget;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> LoseScreenWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* LoseMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FName MainMenuLevelName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    float ReturnToMainMenuDelay;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> PauseMenuWidgetClass;

    UPROPERTY()
    UUserWidget* CurrentPauseMenuWidget;

    FTimerHandle ReturnToMainMenuTimerHandle;

protected:
    UPROPERTY()
    TArray<AKeyEscapeItemBase*> RightHandSlots;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 MaxRightHandSlots;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    int32 CurrentRightHandSlot;

protected:
    UPROPERTY()
    AKeyEscapeFlashlight* EquippedFlashlight;

    protected:
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Keys")
        TArray<FName> CollectedKeys;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    bool bIsDead;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Note")
    bool bIsReadingNote;

    protected:
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
        float NormalFOV;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
        float ZoomFOV;

        UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zoom")
        bool bIsZooming;

private:
    void Move(const FInputActionValue& Value);

    void Look(const FInputActionValue& Value);

    void StartJump();

    void StopJump();

    void StartSprint();

    void StopSprint();

    void Interact();

    void PerformInteractionTrace();

    void ClearInteraction();

    void SelectSlot1();

    void SelectSlot2();

    void SelectSlot3();

    void SelectSlot4();

    void DropInput();

    void ToggleFlashlightInput();

    void UseItemInput();

    void StartZoomInput();

    void StopZoomInput();

    bool IsCurrentItemBinocular() const;

    void ReloadInput();

    void PauseInput();

    AKeyEscapePistolItem* FindPistolInInventory() const;

    void UpdateHUDStatus();

    void Die();

    void ShowLoseScreen();

    void OpenMainMenu();
};