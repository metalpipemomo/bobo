#include "../../engine/GameState/GameState.h"

#include "../../engine/Time.h"

class PauseMenuState : public GameState
{
public:
    void Enter()
    {
        // Initialize pause menu resources
        BOBO_INFO("Entered Pause Menu State");

        Time::SetTimeScale(0);
    }

    void Exit()
    {
        // Clean up pause menu resources
        BOBO_INFO("Exited Pause Menu State");

        Time::SetTimeScale(1);
    }

    void Update()
    {
        // Update pause menu logic
    }

    void Render()
    {
        // Render the pause menu to the screen
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), 0);
        ImGui::SetNextWindowSize(ImVec2(Window::width, Window::height), 0);

        ImGui::Begin("Pause Menu", NULL, MakeFlags(true, true, true, true, true, true, true, false, false, false));

        // Center Text
        std::string text = "Game Paused";
        auto windowWidth = ImGui::GetWindowSize().x;
        auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text(text.c_str());

        // Center Button
        ImGuiStyle& style = ImGui::GetStyle();
        float size = ImGui::CalcTextSize("Resume").x + style.FramePadding.x * 2.0f;
        float avail = ImGui::GetContentRegionAvail().x;
        float off = (avail - size) * .5;
        if (off > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
        if (ImGui::Button("Resume"))
        {
            GameStateManager::EnterGameState(GameStateLabel::IN_GAME);
        }

        // Center Button
        size = ImGui::CalcTextSize("Go to Game Over State").x + style.FramePadding.x * 2.0f;
        avail = ImGui::GetContentRegionAvail().x;
        off = (avail - size) * .5;
        if (off > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
        if (ImGui::Button("Go to Game Over State"))
        {
            GameStateManager::EnterGameState(GameStateLabel::GAME_OVER);
        }

        ImGui::End();
    }
};