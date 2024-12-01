// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AShortStayInLibraryHUD.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Components/Button.h"
#include "Player/MainCharacter.h"

bool UAShortStayInLibraryHUD::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (ResumeGameButton)
	{
		ResumeGameButton->OnClicked.AddDynamic(this, &UAShortStayInLibraryHUD::OnResumeGameButtonClicked);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UAShortStayInLibraryHUD::OnMainMenuButtonClicked);
	}

	if (MainMenuButton2)
	{
		MainMenuButton2->OnClicked.AddDynamic(this, &UAShortStayInLibraryHUD::OnMainMenuButtonClicked);
	}

	if (MainMenuButton3) 
	{
		MainMenuButton3->OnClicked.AddDynamic(this, &UAShortStayInLibraryHUD::OnMainMenuButtonClicked);
	}

	if (RestartLevelButton) 
	{
		RestartLevelButton->OnClicked.AddDynamic(this, &UAShortStayInLibraryHUD::OnRestartLevelButtonClicked);
	}

	if (ControlsButton)
	{
		ControlsButton->OnClicked.AddDynamic(this, &UAShortStayInLibraryHUD::OnControlsButtonClicked);
	}

	if (BackFromControlsButton) 
	{
		BackFromControlsButton->OnClicked.AddDynamic(this, &UAShortStayInLibraryHUD::OnBackFromControlsButtonClicked);
	}

	return true;
}

void UAShortStayInLibraryHUD::SetHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar) 
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
	if (HealthText) 
	{
		FString HealthString = FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, MaxHealth);
		HealthText->SetText(FText::FromString(HealthString));
	}
}

void UAShortStayInLibraryHUD::SetEnergy(float CurrentEnergy, float MaxEnergy)
{
	if (EnergyBar) 
	{
		EnergyBar->SetPercent(CurrentEnergy / MaxEnergy);
	}
	if (EnergyText) 
	{
		FString EnergyString = FString::Printf(TEXT("%.0f / %.0f"), CurrentEnergy, MaxEnergy);
		EnergyText->SetText(FText::FromString(EnergyString));
	}
}

void UAShortStayInLibraryHUD::SetWilloWispCoolDown(float progress)
{
	if (WilloWispCoolDownIcon) 
	{
		WilloWispCoolDownIcon->SetPercent(progress);
	}
}

void UAShortStayInLibraryHUD::SetBeamCoolDown(float progress)
{
	if (BeamCoolDownIcon)
	{
		BeamCoolDownIcon->SetPercent(progress);
	}
}

void UAShortStayInLibraryHUD::SetUltimateCoolDown(float progress)
{
	if (UltimateCoolDownIcon)
	{
		UltimateCoolDownIcon->SetPercent(progress);
	}
}

void UAShortStayInLibraryHUD::ShowGameOverPanel()
{
	DeathPanel->SetVisibility(ESlateVisibility::Visible);
	SetShowCursor(true);
	ChangePauseGameStatus(true);
}

void UAShortStayInLibraryHUD::ChangeVisibilityBookInteractText(bool IsVisible)
{
	if (BookInteractText) 
	{
		if (IsVisible) 
		{
			BookInteractText->SetVisibility(ESlateVisibility::Visible);
		}
		else 
		{
			BookInteractText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UAShortStayInLibraryHUD::ChangeVisibilityEnergySourceInteractText(bool IsVisible)
{
	if (EnergySourceInteractText) 
	{
		if (IsVisible) 
		{
			EnergySourceInteractText->SetVisibility(ESlateVisibility::Visible);
		}
		else 
		{
			EnergySourceInteractText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UAShortStayInLibraryHUD::ChangeVisibilityWinningScreen(bool ShouldBeVisible)
{
	if (WinningScreenPanel) 
	{
		if (ShouldBeVisible) 
		{
			WinningScreenPanel->SetVisibility(ESlateVisibility::Visible);
			SetShowCursor(true);
			ChangePauseGameStatus(true);
		}
		else 
		{
			WinningScreenPanel->SetVisibility(ESlateVisibility::Hidden);
			SetShowCursor(false);
			ChangePauseGameStatus(false);
		}
	}
}

bool UAShortStayInLibraryHUD::GetPauseMenuVisible()
{
	return PauseMenuPanel->IsVisible();
}

void UAShortStayInLibraryHUD::SetPauseMenuVisibility(bool IsVisible)
{
	if (IsVisible) 
	{
		PauseMenuPanel->SetVisibility(ESlateVisibility::Visible);
		SetShowCursor(true);
		ChangePauseGameStatus(true);
	}
	else 
	{
		PauseMenuPanel->SetVisibility(ESlateVisibility::Hidden);
		SetShowCursor(false);
		ChangePauseGameStatus(false);
	}
}

void UAShortStayInLibraryHUD::SetPlayerController(APlayerController* Controller)
{
	PlayerController = Controller; 
}

void UAShortStayInLibraryHUD::SetMainCharacter(AMainCharacter* Character)
{
	MainCharacter = Character;
}

void UAShortStayInLibraryHUD::SetShowCursor(bool ShouldShow)
{
	if (PlayerController)
	{
		if (ShouldShow)
		{
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;
			FInputModeGameAndUI InputMode;
			//FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(this->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
		}
		else
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableMouseOverEvents = false;
			FInputModeGameOnly InputMode;
			/*InputMode.SetWidgetToFocus(this->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);*/
			PlayerController->SetInputMode(InputMode);
		}
	}
}

void UAShortStayInLibraryHUD::SetShowControlsPanel(bool ShouldShow)
{
	if (ControlsPanel) 
	{
		if (ShouldShow) 
		{
			ControlsPanel->SetVisibility(ESlateVisibility::Visible);
		}
		else 
		{
			ControlsPanel->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UAShortStayInLibraryHUD::OnResumeGameButtonClicked()
{
	PauseMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	SetShowCursor(false);
	ChangePauseGameStatus(false);
}

void UAShortStayInLibraryHUD::OnMainMenuButtonClicked()
{
	PauseMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	SetShowCursor(false);
	ChangePauseGameStatus(false);
	UGameplayStatics::OpenLevel(this, FName(TEXT("MenuLevelCorrect")));
}

void UAShortStayInLibraryHUD::OnRestartLevelButtonClicked()
{
	//MainCharacter->ResetPlayerStatus();
	DeathPanel->SetVisibility(ESlateVisibility::Hidden);
	SetShowCursor(false);
	ChangePauseGameStatus(false);
	//TODO: set the name to be the name of the correct level
	UGameplayStatics::OpenLevel(this, FName(TEXT("Library")));
}

void UAShortStayInLibraryHUD::OnControlsButtonClicked()
{
	SetShowControlsPanel(true);
}

void UAShortStayInLibraryHUD::OnBackFromControlsButtonClicked()
{
	SetShowControlsPanel(false);
}

void UAShortStayInLibraryHUD::ChangePauseGameStatus(bool ShouldPause)
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::SetGamePaused(World, ShouldPause);
		//APlayerController* PlayerController = World->GetFirstPlayerController();
		/*if (ShouldPause)
		{
			if (PlayerController)
			{
				PlayerController->SetInputMode(FInputModeUIOnly());
			}
		}
		else 
		{
			if (PlayerController)
			{
				PlayerController->SetInputMode(FInputModeGameAndUI());
			}	
		}*/
	}
}
