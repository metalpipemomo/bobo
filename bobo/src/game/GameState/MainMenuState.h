#include "../../engine/GameState/GameState.h"

class MainMenuState : public GameState
{
public:
    void Enter()
    {
        // Initialize main menu resources
        BOBO_INFO("Entered Main Menu State");
    }

    void Exit()
    {
        // Clean up main menu resources
    }

    void Update()
    {
        // Update main menu logic, e.g., navigation, button selection
    }

    void Render()
    {
        // Render main menu to the screen
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + (Window::width / 2) - 200, main_viewport->WorkPos.y + 25), 0);
        ImGui::SetNextWindowSize(ImVec2(400, 125), 0);

        ImGui::Begin("Main Menu", NULL, MakeFlags(false, true, true, true, true, true, true, false, false, false));

        // Center Text
        std::string text = "Totally Accurate Pool Simulator (Main Menu)";
        auto windowWidth = ImGui::GetWindowSize().x;
        auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text(text.c_str());
       
        // Center Button
        ImGuiStyle& style = ImGui::GetStyle();
        float size = ImGui::CalcTextSize("Play").x + style.FramePadding.x * 2.0f;
        float avail = ImGui::GetContentRegionAvail().x;
        float off = (avail - size) * .5;
        if (off > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        if (ImGui::Button("Play"))
        {
            GameStateManager::EnterGameState(GameStateLabel::IN_GAME);
        }

        // Center Button
        size = ImGui::CalcTextSize("Exit").x + style.FramePadding.x * 2.0f;
        avail = ImGui::GetContentRegionAvail().x;
        off = (avail - size) * .5;
        if (off > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        if (ImGui::Button("Exit"))
        {
            exit(-1);
        }

        ImGui::End();
    }
};