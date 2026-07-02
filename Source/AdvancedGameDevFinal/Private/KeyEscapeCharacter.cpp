#include "KeyEscapeCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KeyEscapeFlashlight.h"
#include "KeyEscapeHUDWidget.h"
#include "KeyEscapeInteractable.h"
#include "KeyEscapeItemBase.h"
#include "KeyEscapeBinocularItem.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "KeyEscapePistolItem.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AKeyEscapeCharacter::AKeyEscapeCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    WalkSpeed = 350.0f;
    SprintSpeed = 650.0f;
    InteractionDistance = 350.0f;

    MaxRightHandSlots = 4;
    CurrentRightHandSlot = 0;

    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
    bIsDead = false;
    bIsReadingNote = false;
    MainMenuLevelName = FName("L_MainMenu");
    ReturnToMainMenuDelay = 3.0f;

    NormalFOV = 90.0f;
    ZoomFOV = 35.0f;
    bIsZooming = false;

    CurrentInteractableActor = nullptr;
    HUDWidget = nullptr;
    EquippedFlashlight = nullptr;

    PauseAction = nullptr;
    PauseMenuWidgetClass = nullptr;
    CurrentPauseMenuWidget = nullptr;

    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    LoseMusic = nullptr;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
    FirstPersonCamera->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f));
    FirstPersonCamera->bUsePawnControlRotation = true;
    FirstPersonCamera->SetFieldOfView(NormalFOV);

    LeftHandPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LeftHandPoint"));
    LeftHandPoint->SetupAttachment(FirstPersonCamera);
    LeftHandPoint->SetRelativeLocation(FVector(50.0f, -20.0f, -20.0f));

    RightHandPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RightHandPoint"));
    RightHandPoint->SetupAttachment(FirstPersonCamera);
    RightHandPoint->SetRelativeLocation(FVector(50.0f, 20.0f, -20.0f));
}

void AKeyEscapeCharacter::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;
    bIsDead = false;
    bIsReadingNote = false;

    bIsZooming = false;

    if (FirstPersonCamera)
    {
        FirstPersonCamera->SetFieldOfView(NormalFOV);
    }

    RightHandSlots.SetNum(MaxRightHandSlots);

    APlayerController* PlayerController = Cast<APlayerController>(GetController());

    if (PlayerController)
    {
        FInputModeGameOnly GameInputMode;
        PlayerController->SetInputMode(GameInputMode);
        PlayerController->bShowMouseCursor = false;

        ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

        if (LocalPlayer)
        {
            UEnhancedInputLocalPlayerSubsystem* Subsystem =
                LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

            if (Subsystem && DefaultMappingContext)
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
        }

        if (HUDWidgetClass)
        {
            HUDWidget = CreateWidget<UKeyEscapeHUDWidget>(PlayerController, HUDWidgetClass);

            if (HUDWidget)
            {
                HUDWidget->AddToViewport();
                HUDWidget->HideInteractionPrompt();
            }
        }
    }
}

void AKeyEscapeCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!bIsDead)
    {
        if (!bIsReadingNote)
        {
            PerformInteractionTrace();
        }

        UpdateHUDStatus();
    }
}

void AKeyEscapeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

    if (EnhancedInputComponent)
    {
        if (MoveAction)
        {
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKeyEscapeCharacter::Move);
        }

        if (LookAction)
        {
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKeyEscapeCharacter::Look);
        }

        if (JumpAction)
        {
            EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AKeyEscapeCharacter::StartJump);
            EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AKeyEscapeCharacter::StopJump);
        }

        if (SprintAction)
        {
            EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AKeyEscapeCharacter::StartSprint);
            EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AKeyEscapeCharacter::StopSprint);
        }

        if (InteractAction)
        {
            EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AKeyEscapeCharacter::Interact);
        }

        if (SelectSlot1Action)
        {
            EnhancedInputComponent->BindAction(SelectSlot1Action, ETriggerEvent::Started, this, &AKeyEscapeCharacter::SelectSlot1);
        }

        if (SelectSlot2Action)
        {
            EnhancedInputComponent->BindAction(SelectSlot2Action, ETriggerEvent::Started, this, &AKeyEscapeCharacter::SelectSlot2);
        }

        if (SelectSlot3Action)
        {
            EnhancedInputComponent->BindAction(SelectSlot3Action, ETriggerEvent::Started, this, &AKeyEscapeCharacter::SelectSlot3);
        }

        if (SelectSlot4Action)
        {
            EnhancedInputComponent->BindAction(SelectSlot4Action, ETriggerEvent::Started, this, &AKeyEscapeCharacter::SelectSlot4);
        }

        if (DropAction)
        {
            EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &AKeyEscapeCharacter::DropInput);
        }

        if (ToggleFlashlightAction)
        {
            EnhancedInputComponent->BindAction(ToggleFlashlightAction, ETriggerEvent::Started, this, &AKeyEscapeCharacter::ToggleFlashlightInput);
        }

        if (UseItemAction)
        {
            EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Started, this, &AKeyEscapeCharacter::UseItemInput);
        }

        if (ZoomAction)
        {
            EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &AKeyEscapeCharacter::StartZoomInput);

            EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &AKeyEscapeCharacter::StopZoomInput);

            EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Canceled, this, &AKeyEscapeCharacter::StopZoomInput);
        }

        if (ReloadAction)
        {
            EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AKeyEscapeCharacter::ReloadInput);
        }

        if (PauseAction)
        {
            EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AKeyEscapeCharacter::PauseInput);
        }
    }
}

void AKeyEscapeCharacter::Move(const FInputActionValue& Value)
{
    if (bIsDead)
    {
        return;
    }

    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller)
    {
        AddMovementInput(GetActorForwardVector(), MovementVector.Y);
        AddMovementInput(GetActorRightVector(), MovementVector.X);
    }
}

void AKeyEscapeCharacter::Look(const FInputActionValue& Value)
{
    if (bIsDead)
    {
        return;
    }

    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void AKeyEscapeCharacter::StartJump()
{
    if (!bIsDead)
    {
        Jump();
    }
}

void AKeyEscapeCharacter::StopJump()
{
    StopJumping();
}

void AKeyEscapeCharacter::StartSprint()
{
    if (!bIsDead)
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void AKeyEscapeCharacter::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AKeyEscapeCharacter::PerformInteractionTrace()
{
    if (!FirstPersonCamera)
    {
        ClearInteraction();
        return;
    }

    FVector Start = FirstPersonCamera->GetComponentLocation();
    FVector End = Start + FirstPersonCamera->GetForwardVector() * InteractionDistance;

    FHitResult HitResult;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility,
        QueryParams
    );

    if (!bHit)
    {
        ClearInteraction();
        return;
    }

    AActor* HitActor = HitResult.GetActor();

    if (!HitActor)
    {
        ClearInteraction();
        return;
    }

    bool bCanInteract = HitActor->GetClass()->ImplementsInterface(
        UKeyEscapeInteractable::StaticClass()
    );

    if (!bCanInteract)
    {
        ClearInteraction();
        return;
    }

    CurrentInteractableActor = HitActor;

    FText PromptText = IKeyEscapeInteractable::Execute_GetInteractionText(HitActor);

    if (HUDWidget)
    {
        HUDWidget->ShowInteractionPrompt(PromptText);
    }
}

void AKeyEscapeCharacter::Interact()
{
    if (bIsDead || bIsReadingNote)
    {
        return;
    }

    if (!CurrentInteractableActor)
    {
        return;
    }

    bool bCanInteract = CurrentInteractableActor->GetClass()->ImplementsInterface(
        UKeyEscapeInteractable::StaticClass()
    );

    if (!bCanInteract)
    {
        return;
    }

    IKeyEscapeInteractable::Execute_Interact(CurrentInteractableActor, this);
}

void AKeyEscapeCharacter::ClearInteraction()
{
    CurrentInteractableActor = nullptr;

    if (HUDWidget)
    {
        HUDWidget->HideInteractionPrompt();
    }
}

bool AKeyEscapeCharacter::AddRightHandItem(AKeyEscapeItemBase* ItemToAdd)
{
    if (bIsDead || !ItemToAdd || !RightHandPoint)
    {
        return false;
    }

    int32 EmptySlotIndex = INDEX_NONE;

    for (int32 i = 0; i < RightHandSlots.Num(); i++)
    {
        if (RightHandSlots[i] == nullptr)
        {
            EmptySlotIndex = i;
            break;
        }
    }

    if (EmptySlotIndex == INDEX_NONE)
    {
        return false;
    }

    RightHandSlots[EmptySlotIndex] = ItemToAdd;

    ItemToAdd->OnPickedUp(this, RightHandPoint);

    SelectRightHandSlot(EmptySlotIndex);

    return true;
}

void AKeyEscapeCharacter::SelectRightHandSlot(int32 SlotIndex)
{
    if (bIsDead)
    {
        return;
    }

    StopZoomInput();

    if (SlotIndex < 0 || SlotIndex >= RightHandSlots.Num())
    {
        return;
    }

    CurrentRightHandSlot = SlotIndex;

    for (int32 i = 0; i < RightHandSlots.Num(); i++)
    {
        if (RightHandSlots[i])
        {
            bool bShouldBeVisible = i == CurrentRightHandSlot;
            RightHandSlots[i]->SetItemVisibleInHand(bShouldBeVisible);
        }
    }
}

void AKeyEscapeCharacter::DropCurrentRightHandItem()
{
    if (bIsDead || !FirstPersonCamera)
    {
        return;
    }

    StopZoomInput();

    if (!RightHandSlots.IsValidIndex(CurrentRightHandSlot))
    {
        return;
    }

    AKeyEscapeItemBase* CurrentItem = RightHandSlots[CurrentRightHandSlot];

    if (!CurrentItem)
    {
        return;
    }

    RightHandSlots[CurrentRightHandSlot] = nullptr;

    FVector DropLocation =
        FirstPersonCamera->GetComponentLocation() +
        FirstPersonCamera->GetForwardVector() * 150.0f;

    DropLocation.Z -= 30.0f;

    FRotator DropRotation = GetActorRotation();

    CurrentItem->OnDropped(DropLocation, DropRotation);

    SelectRightHandSlot(CurrentRightHandSlot);
}

void AKeyEscapeCharacter::UseCurrentRightHandItem()
{
    if (bIsDead)
    {
        return;
    }

    if (!RightHandSlots.IsValidIndex(CurrentRightHandSlot))
    {
        return;
    }

    AKeyEscapeItemBase* CurrentItem = RightHandSlots[CurrentRightHandSlot];

    if (!CurrentItem)
    {
        return;
    }

    CurrentItem->UseItem(this);
}

void AKeyEscapeCharacter::ConsumeCurrentRightHandItem()
{
    if (!RightHandSlots.IsValidIndex(CurrentRightHandSlot))
    {
        return;
    }

    AKeyEscapeItemBase* CurrentItem = RightHandSlots[CurrentRightHandSlot];

    if (!CurrentItem)
    {
        return;
    }

    RightHandSlots[CurrentRightHandSlot] = nullptr;

    CurrentItem->Destroy();

    SelectRightHandSlot(CurrentRightHandSlot);
}

bool AKeyEscapeCharacter::PickupFlashlight(AKeyEscapeFlashlight* FlashlightToPickup)
{
    if (bIsDead || !FlashlightToPickup || !LeftHandPoint)
    {
        return false;
    }

    if (EquippedFlashlight)
    {

        return false;
    }

    EquippedFlashlight = FlashlightToPickup;
    EquippedFlashlight->OnPickedUp(this, LeftHandPoint);

    return true;
}

void AKeyEscapeCharacter::AddFlashlightBattery(float Amount)
{
    if (bIsDead)
    {
        return;
    }

    if (!EquippedFlashlight)
    {

        return;
    }

    EquippedFlashlight->AddBattery(Amount);
}

void AKeyEscapeCharacter::TakePlayerDamage(float DamageAmount)
{
    if (bIsDead)
    {
        return;
    }

    if (DamageAmount <= 0.0f)
    {
        return;
    }

    CurrentHealth -= DamageAmount;

    if (CurrentHealth < 0.0f)
    {
        CurrentHealth = 0.0f;
    }

    if (CurrentHealth <= 0.0f)
    {
        Die();
    }
}

void AKeyEscapeCharacter::HealPlayer(float HealAmount)
{
    if (bIsDead)
    {
        return;
    }

    if (HealAmount <= 0.0f)
    {
        return;
    }

    CurrentHealth += HealAmount;

    if (CurrentHealth > MaxHealth)
    {
        CurrentHealth = MaxHealth;
    }
}

float AKeyEscapeCharacter::GetCurrentHealth() const
{
    return CurrentHealth;
}

float AKeyEscapeCharacter::GetMaxHealth() const
{
    return MaxHealth;
}

bool AKeyEscapeCharacter::IsDead() const
{
    return bIsDead;
}

void AKeyEscapeCharacter::Die()
{
    if (bIsDead)
    {
        return;
    }

    bIsDead = true;

    GetCharacterMovement()->StopMovementImmediately();
    GetCharacterMovement()->DisableMovement();

    ClearInteraction();

    ShowLoseScreen();

    GetWorldTimerManager().SetTimer(
        ReturnToMainMenuTimerHandle,
        this,
        &AKeyEscapeCharacter::OpenMainMenu,
        ReturnToMainMenuDelay,
        false
    );
}

void AKeyEscapeCharacter::ToggleFlashlightInput()
{
    if (!bIsDead && EquippedFlashlight)
    {
        EquippedFlashlight->ToggleFlashlight();
    }
}

void AKeyEscapeCharacter::SelectSlot1()
{
    SelectRightHandSlot(0);
}

void AKeyEscapeCharacter::SelectSlot2()
{
    SelectRightHandSlot(1);
}

void AKeyEscapeCharacter::SelectSlot3()
{
    SelectRightHandSlot(2);
}

void AKeyEscapeCharacter::SelectSlot4()
{
    SelectRightHandSlot(3);
}

void AKeyEscapeCharacter::DropInput()
{
    DropCurrentRightHandItem();
}

void AKeyEscapeCharacter::UseItemInput()
{
    UseCurrentRightHandItem();
}

void AKeyEscapeCharacter::AddKey(FName KeyID)
{
    if (KeyID.IsNone())
    {
        return;
    }

    if (!CollectedKeys.Contains(KeyID))
    {
        CollectedKeys.Add(KeyID);
    }
}

bool AKeyEscapeCharacter::HasKey(FName KeyID) const
{
    if (KeyID.IsNone())
    {
        return true;
    }

    return CollectedKeys.Contains(KeyID);
}

void AKeyEscapeCharacter::ShowLoseScreen()
{
    APlayerController* PlayerController = Cast<APlayerController>(GetController());

    if (!PlayerController)
    {
        return;
    }

    if (LoseMusic)
    {
        UGameplayStatics::PlaySound2D(
            this,
            LoseMusic
        );
    }

    if (LoseScreenWidgetClass)
    {
        UUserWidget* LoseWidget = CreateWidget<UUserWidget>(
            PlayerController,
            LoseScreenWidgetClass
        );

        if (LoseWidget)
        {
            LoseWidget->AddToViewport();
        }
    }

    PlayerController->SetInputMode(FInputModeUIOnly());
    PlayerController->bShowMouseCursor = true;
}

void AKeyEscapeCharacter::OpenMainMenu()
{
    if (!MainMenuLevelName.IsNone())
    {
        UGameplayStatics::OpenLevel(this, MainMenuLevelName);
    }
}

void AKeyEscapeCharacter::StartZoomInput()
{
    if (bIsDead)
    {
        return;
    }

    if (!FirstPersonCamera)
    {
        return;
    }

    if (!IsCurrentItemBinocular())
    {
        return;
    }

    bIsZooming = true;
    FirstPersonCamera->SetFieldOfView(ZoomFOV);
}

void AKeyEscapeCharacter::StopZoomInput()
{
    if (!FirstPersonCamera)
    {
        return;
    }

    bIsZooming = false;
    FirstPersonCamera->SetFieldOfView(NormalFOV);
}

bool AKeyEscapeCharacter::IsCurrentItemBinocular() const
{
    if (!RightHandSlots.IsValidIndex(CurrentRightHandSlot))
    {
        return false;
    }

    AKeyEscapeItemBase* CurrentItem = RightHandSlots[CurrentRightHandSlot];

    if (!CurrentItem)
    {
        return false;
    }

    return Cast<AKeyEscapeBinocularItem>(CurrentItem) != nullptr;
}

bool AKeyEscapeCharacter::AddPistolAmmo(int32 AmmoAmount)
{
    if (bIsDead)
    {
        return false;
    }

    AKeyEscapePistolItem* Pistol = FindPistolInInventory();

    if (!Pistol)
    {

        return false;
    }

    Pistol->AddReserveAmmo(AmmoAmount);

    return true;
}

UCameraComponent* AKeyEscapeCharacter::GetFirstPersonCamera() const
{
    return FirstPersonCamera;
}

void AKeyEscapeCharacter::ReloadInput()
{
    if (bIsDead)
    {
        return;
    }

    if (!RightHandSlots.IsValidIndex(CurrentRightHandSlot))
    {
        return;
    }

    AKeyEscapeItemBase* CurrentItem = RightHandSlots[CurrentRightHandSlot];

    if (!CurrentItem)
    {
        return;
    }

    CurrentItem->ReloadItem(this);
}

AKeyEscapePistolItem* AKeyEscapeCharacter::FindPistolInInventory() const
{
    for (AKeyEscapeItemBase* Item : RightHandSlots)
    {
        AKeyEscapePistolItem* Pistol = Cast<AKeyEscapePistolItem>(Item);

        if (Pistol)
        {
            return Pistol;
        }
    }

    return nullptr;
}

void AKeyEscapeCharacter::UpdateHUDStatus()
{
    if (!HUDWidget)
    {
        return;
    }

    float HealthPercent = 0.0f;

    if (MaxHealth > 0.0f)
    {
        HealthPercent = CurrentHealth / MaxHealth;
    }

    float BatteryPercent = 0.0f;

    if (EquippedFlashlight && EquippedFlashlight->GetMaxBattery() > 0.0f)
    {
        BatteryPercent =
            EquippedFlashlight->GetCurrentBattery() /
            EquippedFlashlight->GetMaxBattery();
    }

    FText ItemTextValue = FText::FromString("Item: Empty");
    FText AmmoTextValue = FText::FromString("Ammo: -");

    if (RightHandSlots.IsValidIndex(CurrentRightHandSlot))
    {
        AKeyEscapeItemBase* CurrentItem = RightHandSlots[CurrentRightHandSlot];

        if (CurrentItem)
        {
            ItemTextValue = FText::FromString(
                FString::Printf(
                    TEXT("Item: %s"),
                    *CurrentItem->GetItemName().ToString()
                )
            );

            AKeyEscapePistolItem* Pistol = Cast<AKeyEscapePistolItem>(CurrentItem);

            if (Pistol)
            {
                AmmoTextValue = FText::FromString(
                    FString::Printf(
                        TEXT("Ammo: %d / %d"),
                        Pistol->GetCurrentAmmo(),
                        Pistol->GetReserveAmmo()
                    )
                );
            }
        }
    }

    HUDWidget->UpdateStatusBars(
        HealthPercent,
        BatteryPercent,
        ItemTextValue,
        AmmoTextValue
    );
}

void AKeyEscapeCharacter::SetReadingNote(bool bReading)
{
    bIsReadingNote = bReading;

    if (bIsReadingNote)
    {
        ClearInteraction();
    }
}

void AKeyEscapeCharacter::PauseInput()
{
    if (bIsDead || bIsReadingNote)
    {
        return;
    }

    if (!PauseMenuWidgetClass)
    {
        return;
    }

    APlayerController* PlayerController = Cast<APlayerController>(GetController());

    if (!PlayerController)
    {
        return;
    }

    if (CurrentPauseMenuWidget && CurrentPauseMenuWidget->IsInViewport())
    {
        return;
    }

    ClearInteraction();

    CurrentPauseMenuWidget = CreateWidget<UUserWidget>(
        PlayerController,
        PauseMenuWidgetClass
    );

    if (!CurrentPauseMenuWidget)
    {
        return;
    }

    CurrentPauseMenuWidget->AddToViewport();

    FInputModeUIOnly UIInputMode;
    UIInputMode.SetWidgetToFocus(CurrentPauseMenuWidget->TakeWidget());

    PlayerController->SetInputMode(UIInputMode);
    PlayerController->bShowMouseCursor = true;

    UGameplayStatics::SetGamePaused(this, true);
}