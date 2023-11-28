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

        MakeCenterText("Totally Accurate Pool Simulator (Main Menu)");
       
        LowerCursor();

        if (MakeCenterButton("Play"))
        {
            GameStateManager::EnterGameState(GameStateLabel::IN_GAME);
        }

        LowerCursor();

        if (MakeCenterButton("Exit"))
        {
            exit(-1);
        }

        ImGui::End();
    }
};