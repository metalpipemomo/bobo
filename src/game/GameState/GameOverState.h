#include "../../engine/GameState/GameState.h"

class GameOverState : public GameState
{
public:
    void Enter()
    {
        // Initialize game over resources
        BOBO_INFO("Entered Game Over State");

        // Check who won
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
        ImGui::SetNextWindowSize(ImVec2(Window::width, Window::height), 0);

        ImGui::Begin("Game Over", NULL, MakeFlags(true, true, true, true, true, true, true, false, false, false));

        // Center Text
        std::string text = "Congratulations " + winner + "!";
        auto windowWidth = ImGui::GetWindowSize().x;
        auto windowHeight = ImGui::GetWindowSize().y;
        auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
        auto textHeight = ImGui::CalcTextSize(text.c_str()).y;
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::SetCursorPosY((windowHeight - textHeight) * 0.5f);
        ImGui::Text(text.c_str());

        // Center Button
        ImGuiStyle& style = ImGui::GetStyle();
        float size = ImGui::CalcTextSize("Return to Main Menu").x + style.FramePadding.x * 2.0f;
        float avail = ImGui::GetContentRegionAvail().x;
        float off = (avail - size) * .5;
        if (off > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        if (ImGui::Button("Return to Main Menu"))
        {
            GameStateManager::ResetGameStateStack();
            GameStateManager::EnterGameState(GameStateLabel::MAIN_MENU);
        }

        ImGui::End();
    }

    void SetWinner(std::string winner)
    {
        this->winner = winner;
    }

private:
    std::string winner;
};