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
        ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT), 0);

        ImGui::StyleColorsClassic();
        ImGui::Begin("Pause Menu", NULL, ImGuiHelpers::MakeFlags(true, true, true, true, true, true, true, false, false, false));

        ImGuiHelpers::LowerCursor(25);

        ImGuiHelpers::MakeCenterText("Game Paused", true, false);
        ImGuiHelpers::LowerCursor();

        if (ImGuiHelpers::MakeCenterButton("Resume"))
        {
            GameStateManager::TogglePauseState();
        }

        ImGuiHelpers::LowerCursor(50);

        ImGuiHelpers::MakeCenterText("Controls", true, false);

        ImGuiHelpers::LowerCursor();

        ImGuiHelpers::MakeVerticalList(
            {
                "Rotate Cue L/R - 1 <-> 2",
                "Fine Adjustment - Hold Ctrl while Rotating Cue",
                "Fast Forward - F",
                "Charge Shot - Hold Space"
            }, 10, "Gameplay");

        ImGuiHelpers::LowerCursor();

        ImGuiHelpers::MakeVerticalList(
            {
                "Move F/B - W <-> S", 
                "Move L/R - A <-> D",
                "Confirm Placement - P",
            }, 10, "Re-placing Cue Ball");

        ImGuiHelpers::LowerCursor();

        ImGuiHelpers::MakeVerticalList(
            {
                "Rotate Camera L/R - A <-> D",
                "Zoom Camera - W <-> S",
                "Toggle Camera Mode - Tab (Switch between Cue Ball and Table Focus)",
                "Free Camera - Ctrl + Tab",
            }, 10, "Camera");

        ImGui::End();
    }
};