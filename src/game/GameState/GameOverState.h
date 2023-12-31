#include "../../engine/GameState/GameState.h"

class GameOverState : public GameState
{
public:
    void Enter()
    {
        // Initialize game over resources
        BOBO_INFO("Entered Game Over State");

        NotificationManager::ClearPendingBannerNotifications();
    }

    void Exit()
    {
        // Clean up game over resources
    }

    void Update()
    {
        // Update game over logic, such as checking for restart or exit
    }

    void Render()
    {
        // Render the game over screen to the display
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), 0);
        ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT), 0);

        ImGui::StyleColorsClassic();
        ImGui::Begin("Game Over", NULL, ImGuiHelpers::MakeFlags(true, true, true, true, true, true, true, false, false, false));

        ImGuiHelpers::MakeCenterText("Congratulations " + winner + "!", true, true);
        ImGuiHelpers::LowerCursor();

        ImGuiHelpers::MakeCenterText("Won via: " + winReason, true);
        ImGuiHelpers::LowerCursor();

        if (ImGuiHelpers::MakeCenterButton("Return to Main Menu"))
        {
            GameStateManager::ResetGameStateStack();
            GameStateManager::EnterGameState(GameStateLabel::MAIN_MENU);
        }

        ImGui::End();
    }

    void SetWinner(std::string winner, std::string winReason)
    {
        this->winner = winner;
        this->winReason = winReason;
    }

private:
    std::string winner;
    std::string winReason;
};