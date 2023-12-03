#include "../../engine/GameState/GameState.h"

#include "../../engine/Notifications/NotificationManager.h"

#include <string>
#include "../../engine/Time.h"

enum Turn { P1, P2 };

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
        m_ShotPower = 0.0f;
        m_ShotPowerDirection = 1;

        m_Turn = Turn::P1;

        PopupManager::MakePopup(
            "How to Use your Cue",
            std::vector<PerPopupInformation>
            {
                PerPopupInformation("Hold Space to Charge your Shot.The Power will\nbounce back and forth between full and empty.",
                    ImVec2(400, 117.5)),
                PerPopupInformation("Adjust the Cue's angle using...?", ImVec2(400, 105)),
                PerPopupInformation("Happy Pooling Gamer!", ImVec2(400, 105))
            },
            AnchorPos::TOP_CENTER,
            ImVec2(0, 25)
        );
    }

    void Exit()
    {
        // Clean up game over resources
        BOBO_INFO("Exited In-Game State");
    }

    void Update()
    {
        if (m_Turn != m_TurnLastUpdate)
        {
            std::string newTurn;
            if (m_Turn == Turn::P1)
            {
                newTurn = "P1";
            }
            else
            {
                newTurn = "P2";
            }
            NotificationManager::SendSlidingBannerNotification(newTurn + " Turn Start", NotificationTextColor::WHITE);
        }
        m_TurnLastUpdate = m_Turn;

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

        // Solids UI
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 25, main_viewport->WorkPos.y + 25), 0);
        ImGui::SetNextWindowSize(ImVec2(225, 90), 0);

        if (m_Turn == Turn::P1)
        {
            ImGui::StyleColorsLight();
        }
        else
        {
            ImGui::StyleColorsDark();
        }

        ImGui::Begin("P1", NULL, ImGuiHelpers::MakeFlags(false, true, true, true, true, true, true, false, false, false));

        std::string solidLabel = "Solid Balls Remaining " + std::to_string(m_SolidBallsRemaining);
        ImGuiHelpers::MakeCenterText(solidLabel);

        ImGuiHelpers::LowerCursor();

        if (ImGuiHelpers::MakeCenterButton("Sink Solid") && m_Turn == Turn::P1)
        {
            m_SolidBallsRemaining--;
            NotificationManager::SendDefaultNotification("A Solid Ball has been Sunk", NotificationTextColor::BLUE);
            m_Turn = Turn::P2;
        }

        ImGui::End();

        // Striped UI
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + WINDOW_WIDTH - 250, main_viewport->WorkPos.y + 25), 0);
        ImGui::SetNextWindowSize(ImVec2(225, 90), 0);

        if (m_Turn == Turn::P2)
        {
            ImGui::StyleColorsLight();
        }
        else
        {
            ImGui::StyleColorsDark();
        }

        ImGui::Begin("P2", NULL, ImGuiHelpers::MakeFlags(false, true, true, true, true, true, true, false, false, false));

        std::string stripedLabel = "Striped Balls Remaining " + std::to_string(m_StripedBallsRemaining);
        ImGuiHelpers::MakeCenterText(stripedLabel);

        ImGuiHelpers::LowerCursor();

        if (ImGuiHelpers::MakeCenterButton("Sink Striped") && m_Turn == Turn::P2)
        {
            m_StripedBallsRemaining--;
            NotificationManager::SendDefaultNotification("A Striped Ball has been Sunk", NotificationTextColor::RED);
            m_Turn = Turn::P1;
        }

        ImGui::End();

        // Turn UI
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + WINDOW_WIDTH - 225, main_viewport->WorkPos.y + WINDOW_HEIGHT - 50), 0);
        ImGui::SetNextWindowSize(ImVec2(200, 25), 0);

        ImGui::StyleColorsClassic();
        ImGui::Begin("Turn", NULL, ImGuiHelpers::MakeFlags(true, true, true, true, true, true, true, false, false, false));

        // Construct turn label
        std::string currentTurn;
        if (m_Turn == Turn::P1)
            currentTurn = "P1";
        else
            currentTurn = "P2";
        std::string turnLabel = "Shooting: " + currentTurn;
        ImGuiHelpers::MakeCenterText(turnLabel.c_str(), true, true);

        ImGui::End();

        // if space is held or we are still resolving a shot, the bar must be rendered
        // Power Bar
        m_ShootKeyHeld = Input::GetKey(GLFW_KEY_SPACE);

        // set the resolving shot timer upon releasing a shot
        if (!m_ShootKeyHeld && m_ShootKeyHeldLastFrame && m_ResolvingShot <= 0)
        {
            // Has released shot, must resolve shot
            m_ResolvingShot = 1;
        }

        if (m_ShootKeyHeld || m_ResolvingShot > 0)
        {
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + WINDOW_WIDTH / 2 - m_ProgressBarWidth / 2, 
                main_viewport->WorkPos.y + WINDOW_HEIGHT - m_ProgressBarHeight - 25), 0);
            ImGui::SetNextWindowSize(ImVec2(m_ProgressBarWidth + m_ProgressBarPadding, m_ProgressBarHeight + m_ProgressBarPadding), 0);

            ImGuiStyle& style = ImGui::GetStyle();
            ImVec2 oldWindowPadding = style.WindowPadding;
            style.WindowPadding = ImVec2(m_ProgressBarPadding, m_ProgressBarPadding);

            ImVec4 oldPlotHistogramColor = style.Colors[ImGuiCol_PlotHistogram];
            if (m_ResolvingShot > 0)
                style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1, 1, 0, 1);
            else
                style.Colors[ImGuiCol_PlotHistogram] = ImVec4(m_ShotPower, 0, 0, 1);

            // only increase the power bar if a shot is not currently being resolved
            if (m_ShootKeyHeld && m_ResolvingShot <= 0)
            {
                m_ShotPower += m_ShotPowerDirection * Time::DeltaTime();
                if (m_ShotPower >= 1)
                    m_ShotPowerDirection = -1;
                if (m_ShotPower <= 0)
                    m_ShotPowerDirection = 1;
            }
        
            ImGui::Begin("Progress Bar", NULL, ImGuiHelpers::MakeFlags(true, true, true, true, true, true, true, false, false, false));

            // Construct the progress bar
            ImGui::ProgressBar(m_ShotPower, ImVec2(m_ProgressBarWidth - m_ProgressBarPadding, 
                m_ProgressBarHeight - m_ProgressBarPadding));

            // Reset
            style.WindowPadding = oldWindowPadding;
            style.Colors[ImGuiCol_PlotHistogram] = oldPlotHistogramColor;

            ImGui::End();
        }

        // set the shot power to 0 upon the current shot being resolved
        if (m_ResolvingShot > 0)
        {
            float oldResolvingShot = m_ResolvingShot;
            m_ResolvingShot -= Time::DeltaTime();
            if (m_ResolvingShot <= 0 && oldResolvingShot > 0)
            {
                // Has resolved shot
                m_ShotPower = 0;
            }
        }

        m_ShootKeyHeldLastFrame = m_ShootKeyHeld;
    }
private:
    int m_StripedBallsRemaining;
    int m_SolidBallsRemaining;
    Turn m_Turn;
    Turn m_TurnLastUpdate;
    float m_ShotPower;
    int m_ShotPowerDirection;

    int m_ProgressBarWidth = 300;
    int m_ProgressBarHeight = 50;
    int m_ProgressBarPadding = 3;

    float m_ResolvingShot;
    bool m_ShootKeyHeld;
    bool m_ShootKeyHeldLastFrame;
};