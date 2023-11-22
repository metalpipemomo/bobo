#include "../../engine/GameState/GameState.h"

#include <string>

class InGameState : public GameState
{
public:
    void Enter()
    {
        // Initialize game over resources
        BOBO_INFO("Entered In-Game State");

        // Set stuff
        m_SolidBallsRemaining = 8;
        m_StripedBallsRemaining = 8;
    }

    void Exit()
    {
        // Clean up game over resources
        BOBO_INFO("Exited In-Game State");
    }

    void Update()
    {
        // Update game over logic, such as checking for restart or exit
        if (m_SolidBallsRemaining <= 0)
        {
            GameOverState* gameOverState = (GameOverState*)GameStateManager::FetchGameState(GameStateLabel::GAME_OVER);
            gameOverState->SetWinner("Solids");
            GameStateManager::EnterGameState(GameStateLabel::GAME_OVER);
        }

        if (m_StripedBallsRemaining <= 0)
        {
            GameOverState* gameOverState = (GameOverState*)GameStateManager::FetchGameState(GameStateLabel::GAME_OVER);
            gameOverState->SetWinner("Stripes");
            GameStateManager::EnterGameState(GameStateLabel::GAME_OVER);
        }
    }

    void Render()
    {
        // Render the game over screen to the display
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 25, main_viewport->WorkPos.y + 25), 0);
        ImGui::SetNextWindowSize(ImVec2(500, 200), 0);

        ImGui::Begin("Score", NULL, MakeFlags(true, true, true, true, true, true, true, true, false, false));

        ImGui::Text("In-Game");

        // Solids UI
        std::string solidLabel = "Solid Balls Remaining " + std::to_string(m_SolidBallsRemaining);
        ImGui::Text(solidLabel.c_str());
        if (ImGui::Button("Sink Solid"))
        {
            m_SolidBallsRemaining--;
        }

        // Move Cursor down some
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

        // Striped UI
        std::string stripedLabel = "Striped Balls Remaining " + std::to_string(m_StripedBallsRemaining);
        ImGui::Text(stripedLabel.c_str());
        if (ImGui::Button("Sink Striped"))
        {
            m_StripedBallsRemaining--;
        }

        ImGui::End();
    }
private:
        int m_StripedBallsRemaining;
        int m_SolidBallsRemaining;
};