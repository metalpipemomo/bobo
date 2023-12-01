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
        m_BannerAlpha = 0.0f;
        m_BannerTimer = 0.0f;

        m_Turn = Turn::P1;
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
            NotificationManager::SendNotification(newTurn + " Turn Start", NotificationTextColor::WHITE);
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
        ImGui::SetNextWindowSize(ImVec2(200, 80), 0);

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
        ImGui::Text(solidLabel.c_str());

        // Move Cursor down some
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

        if (ImGuiHelpers::MakeCenterButton("Sink Solid") && m_Turn == Turn::P1)
        {
            m_SolidBallsRemaining--;
            NotificationManager::SendNotification("A Solid Ball has been Sunk", NotificationTextColor::BLUE);
            m_Turn = Turn::P2;
            m_BannerTimer = 7.5f;
        }

        ImGui::End();

        // Striped UI
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + WINDOW_WIDTH - 225, main_viewport->WorkPos.y + 25), 0);
        ImGui::SetNextWindowSize(ImVec2(200, 80), 0);

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
        ImGui::Text(stripedLabel.c_str());

        // Move Cursor down some
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

        if (ImGuiHelpers::MakeCenterButton("Sink Striped") && m_Turn == Turn::P2)
        {
            m_StripedBallsRemaining--;
            NotificationManager::SendNotification("A Striped Ball has been Sunk", NotificationTextColor::RED);
            m_Turn = Turn::P1;
            m_BannerTimer = 7.5f;
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
        ImGui::Text(turnLabel.c_str());

        ImGui::End();

        // Banner Notification
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y + WINDOW_HEIGHT / 2 - 200), 0);
        ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, 100), 0);

        ImGui::StyleColorsClassic();
        ImGuiStyle& style = ImGui::GetStyle();
        m_BannerTimer -= 1.0f  * Time::DeltaTime();
        if (m_BannerTimer >= 5.0f) {
            m_BannerAlpha += 0.4f * Time::DeltaTime();
        } else if (m_BannerTimer > 0) {
            m_BannerAlpha -= 0.2f * Time::DeltaTime();
        }
        BOBO_INFO("Timer: {}", m_BannerTimer);
        BOBO_INFO("Alpha: {}", m_BannerAlpha);
        style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, m_BannerAlpha);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, m_BannerAlpha);
        style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, m_BannerAlpha);

        ImGui::Begin("Banner", NULL, ImGuiHelpers::MakeFlags(true, true, true, true, true, true, true, false, false, false));
        ImGui::SetWindowFontScale(4);

        // Construct banner label
        std::string bannerLabel;
        if (m_Turn == Turn::P1)
            bannerLabel = "PLAYER 1'S TURN";
        else
            bannerLabel = "PLAYER 2'S TURN";

        auto textWidth = ImGui::CalcTextSize(bannerLabel.c_str()).x;
        ImGui::SetCursorPosX((WINDOW_WIDTH - textWidth) * 0.5f);
        ImGui::Text(bannerLabel.c_str());

        ImGui::End();

        // Power Bar
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + WINDOW_WIDTH / 2 - 131, main_viewport->WorkPos.y + WINDOW_HEIGHT - 200), 0);
        ImGui::SetNextWindowSize(ImVec2(262, 60), 0);

        style.WindowRounding = 5.3f;
        style.WindowPadding = ImVec2(3.0f, 3.0f);

        // Increase the progress bar if space is being held down
        if (Input::GetKey(GLFW_KEY_SPACE))
        {
            style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
            if (m_ShotPower < 100.0f)
                m_ShotPower += 1.0f  * Time::DeltaTime();
        }
        else
        {
            style.Colors[ImGuiCol_WindowBg] = ImVec4(0.89f, 0.89f, 0.95f, 1.00f);
            m_ShotPower = 0.0f;
        }

        ImGui::Begin("Test", NULL, ImGuiHelpers::MakeFlags(true, true, true, true, true, true, true, false, false, false));

        // Construct the progress bar
        ImGui::ProgressBar(m_ShotPower, ImVec2(256.0f, 54.0f));

        ImGui::End();
    }
private:
    int m_StripedBallsRemaining;
    int m_SolidBallsRemaining;
    Turn m_Turn;
    Turn m_TurnLastUpdate;
    float m_ShotPower;
    float m_BannerAlpha;
    float m_BannerTimer;
};