// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AShortStayInLibraryHUD.generated.h"

class UProgressBar;
class UTextBlock;
class UCanvasPanel;
class APlayerController; 
class UInputAction;
class UButton;
class AMainCharacter;

/**
 * 
 */
UCLASS()
class ASHORTSTAYINLIBRARY_API UAShortStayInLibraryHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

	void SetHealth(float CurrentHealth, float MaxHealth);
	void SetEnergy(float CurrentEnergy, float MaxEnergy);
	void SetWilloWispCoolDown(float progress);
	void SetBeamCoolDown(float progress);
	void SetUltimateCoolDown(float progress);
	void ShowGameOverPanel();
	void ChangeVisibilityBookInteractText(bool);
	void ChangeVisibilityEnergySourceInteractText(bool);
	void ChangeVisibilityWinningScreen(bool);
	bool GetPauseMenuVisible();
	void SetPauseMenuVisibility(bool);
	void SetPlayerController(APlayerController*);
	void SetMainCharacter(AMainCharacter*);
	void SetShowCursor(bool);
	void SetShowControlsPanel(bool);
	void ChangePauseGameStatus(bool);

protected:
	UFUNCTION()
	void OnResumeGameButtonClicked();
	UFUNCTION()
	void OnMainMenuButtonClicked();
	UFUNCTION()
	void OnRestartLevelButtonClicked();
	UFUNCTION()
	void OnControlsButtonClicked();
	UFUNCTION()
	void OnBackFromControlsButtonClicked();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* EnergyBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* BeamCoolDownIcon; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* WilloWispCoolDownIcon; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* UltimateCoolDownIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* HealthText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* EnergyText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* EnergySourceInteractText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* BookInteractText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* PauseMenuPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* WinningScreenPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* DeathPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ResumeGameButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* RestartLevelButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* MainMenuButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* MainMenuButton2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* MainMenuButton3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* BackFromControlsButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ControlsButton;
	APlayerController* PlayerController = nullptr;
	//UInputAction* PauseAction;
	AMainCharacter* MainCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* ControlsPanel;
};
