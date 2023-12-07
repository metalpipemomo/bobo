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
        m_SolidBallsRemaining = 7;
        m_StripedBallsRemaining = 7;
        m_ShotPower = 0.0f;
        m_ShotPowerDirection = 1;
        m_shotAllowedFlag = true;


        m_Turn = Turn::P1;

        PopupManager::MakePopup(
            "How to Use your Cue",
            std::vector<PerPopupInformation>
            {
                PerPopupInformation("Hold Space to Charge your Shot.The Power will\nbounce back and forth between full and empty.",
                    ImVec2(400, 117.5)),
                PerPopupInformation("Adjust the Cue's angle using 1 and 2", ImVec2(400, 105)),
                PerPopupInformation("Happy Pooling Gamer!", ImVec2(400, 105))
            },
            AnchorPos::TOP_CENTER,
            ImVec2(0, 25)
        );

        m_Scene = SceneManager::GetActiveScene();
        auto objects = m_Scene->GetComponentsOfType<ObjectTag>();

        // getting game object components needed
        for (auto& object : objects)
        {
            if (object->tag == "cue")
            {
                m_CueTransform = m_Scene->GetComponent<Transform>(object->m_OwnerId);
                
            }
            if (object->tag == "cueBall")
            {
                m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->SetVelocity({0, 0, 0});
                m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->SetPosition({-0.002514, -1.048805, -2.501616});
                m_BallRb = m_Scene->GetComponent<Rigidbody>(object->m_OwnerId);
                m_CueBallTransform = m_Scene->GetComponent<Transform>(object->m_OwnerId);
                
            }
            if (object->tag == "gamemanager")
            {
                m_Manager = m_Scene->GetComponent<GameManager>(object->m_OwnerId);
                
            }

            //Blast off walls on InGameStateEnter
            if (object->tag == "ceiling")
            {
                m_Scene->GetComponent<RoofMove>(object->m_OwnerId)->StartMove();
            }
            if (object->tag == "leftWallMover")
            {
                m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->SetMotionType(false);
                m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->AddLinearVelocity(JPH::Vec3(80,0,0));
            }
            if (object->tag == "rightWallMover")
            {
                m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->SetMotionType(false);
                m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->AddLinearVelocity(JPH::Vec3(-80,0,0));
            }
            if (object->tag == "backWallMover")
            {
                m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->SetMotionType(false);
                m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->AddLinearVelocity(JPH::Vec3(0,0,-80));
            }

        }

        //Only the balls have been given alt tags, no other entities within the game.

        auto objects2 = m_Scene->GetComponentsOfType<ObjectTagAlt>();

        for(auto& object : objects2){
            m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->SetMotionType(true);
            m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->SetVelocity({0, 0, 0});
            float x = 0.0;
            float y = 0.0;
            float z = 0.0;
            if(object->tag == "Ball_001"){
                x = 0.000000;
                y = -1.047199;
                z = -5.500000;
            } else if(object->tag == "Ball_009"){
                x = -0.210000;
                y = -1.047199;
                z = -5.920382;
            } else if(object->tag == "Ball_002"){
                x = 0.209997;
                y = -1.047199;
                z = -5.920956;
            } else if(object->tag == "Ball_010"){
                x = -0.418573;
                y = -1.047199;
                z = -6.339496;
            } else if(object->tag == "Ball_008"){
                x = -0.000000;
                y = -1.047199;
                z = -6.340000;
            } else if(object->tag == "Ball_003"){
                x = 0.421423;
                y = -1.047199;
                z = -6.339947;
            } else if(object->tag == "Ball_011"){
                x = -0.630001;
                y = -1.047199;
                z = -6.760705;
            } else if(object->tag == "Ball_007"){
                x = -0.210000;
                y = -1.047199;
                z = -6.760676;
            }else if(object->tag == "Ball_014") {
                x = 0.209998;
                y = -1.047199;
                z = -6.760877;
            } else if(object->tag == "Ball_004"){
                x = 0.629998;
                y = -1.047199;
                z = -6.761054;
            } else if(object->tag == "Ball_005"){
                x = -0.840000;
                y = -1.047199;
                z = -7.180000;
            } else if(object->tag == "Ball_013"){
                x = -0.420001;
                y = -1.047199;
                z = -7.181280;
            } else if(object->tag == "Ball_015"){
                x = -0.000002;
                y = -1.047199;
                z = -7.180532;
            } else if(object->tag == "Ball_006"){
                x = 0.418829;
                y = -1.047199;
                z = -7.180407;
            } else if(object->tag == "Ball_012"){
                x = 0.839998;
                y = -1.047199;
                z = -7.180000;
            } else {

            }
            JPH::Vec3 newPos = {x, y, z};
            m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->SetPosition(newPos);
            m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->SetMotionType(false);
        }
    }

    void Exit()
    {
        // Clean up game over resources
        BOBO_INFO("Exited In-Game State");
    }

    void UpdateGame()
    {
        m_StripedBallsRemaining = m_Manager->stripesAmount;
        m_SolidBallsRemaining = m_Manager->solidsAmount;
        auto transform = m_BallRb->GetTransform();
        m_cueBallPos = transform->position;
        m_CueTransform->rotation = glm::vec3(0, m_shotAngle, 0);

        // 1 and 2 keys for rotating shot angle
        if (Input::GetKey(GLFW_KEY_1))
        {
            m_shotAngle += m_rotateSpeed * Time::DeltaTime();
        }
        if (Input::GetKey(GLFW_KEY_2))
        {
            m_shotAngle -= m_rotateSpeed * Time::DeltaTime();
        }

        // if cue ball almost stopped, stop it
        if (m_BallRb->GetVelocity().Length() < 0.3 || !m_BallRb->IsEnabled())
        {
            m_BallRb->SetVelocity(JPH::Vec3{ 0,0,0 });
            // set cue shot indicator and set flag to true
            m_shotAllowedFlag = true;
            m_CueTransform->position = m_CueBallTransform->position + glm::vec3(Sin(m_shotAngle) * -1, 0, Cos(m_shotAngle) * -1);
        }

        // shoot cue ball with space and swap turns
        if (m_shotactivated && m_shotAllowedFlag)
        {
            // make cue shot indicator dissapear and set flag to false
            m_shotAllowedFlag = false;
            m_shotactivated = false;
            m_ShotWasAllowed = false;
            m_HasSunkBadly = false;
            m_CueTransform->position = glm::vec3{ 100,100,100 };
            m_BallRb->addForce(JPH::Vec3(Sin(m_shotAngle) * -m_ShotPower*m_maxShotPower, 0, Cos(m_shotAngle) * -m_ShotPower*m_maxShotPower));
        }
    }

    void Update()
    {
        // do game updating stuff
        UpdateGame();

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

        if (!m_ShotWasAllowed && m_shotAllowedFlag) {
            m_Turn = m_NextTurn;
            m_HasSunkBadly = false;
            m_ShotWasAllowed = true;
        }
    }

    void SinkSolid() 
    {
        NotificationManager::SendDefaultNotification("A Solid Ball has been Sunk", NotificationTextColor::BLUE);
        
        if (m_Turn == Turn::P1 && !m_HasSunkBadly)
        {
            m_NextTurn = Turn::P1;
        }
        if (m_Turn == Turn::P2) 
        {
            m_NextTurn = Turn::P1;
            m_HasSunkBadly = true;
        }
    }

    void SinkStriped() 
    {
        NotificationManager::SendDefaultNotification("A Striped Ball has been Sunk", NotificationTextColor::RED);
        
        if (m_Turn == Turn::P1)
        {
            m_NextTurn = Turn::P2;
            m_HasSunkBadly = true;
        }
        if (m_Turn == Turn::P2 && !m_HasSunkBadly)
        {
            m_NextTurn = Turn::P2;
        }
    }

    void SetWinner(std::string winner) 
    {
        GameOverState* gameOverState = (GameOverState*)GameStateManager::FetchGameState(GameStateLabel::GAME_OVER);
        gameOverState->SetWinner(winner);
        GameStateManager::EnterGameState(GameStateLabel::GAME_OVER);
    }

    void Sink8Ball()
    {
        if (m_SolidBallsRemaining > 0 && m_StripedBallsRemaining > 0) {
            if (m_Turn == Turn::P1)
                SetWinner("Stripes");
            else
                SetWinner("Solids");

            return;
        }

        if (m_SolidBallsRemaining <= 0)
            SetWinner("Solids");
        else
            SetWinner("Stripes");
    }

    void SinkCueBall()
    {
        // unneeded maybe? this is called in game.h but idk if we actually need this
        // if you remove this then remember to remove its call in game.h
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

        m_ShootKeyHeld = Input::GetKey(GLFW_KEY_SPACE) && m_BallRb->IsEnabled();

        // set the resolving shot timer upon releasing a shot
        if (!m_ShootKeyHeld && m_ShootKeyHeldLastFrame && m_ResolvingShot <= 0)
        {
            // Has released shot, must resolve shot
            m_ResolvingShot = 1;
        }

        if (m_shotAllowedFlag && m_ShootKeyHeld || m_ResolvingShot > 0)
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
            m_shotactivated = true;
            float oldResolvingShot = m_ResolvingShot;
            m_ResolvingShot -= Time::DeltaTime();
            if (m_ResolvingShot <= 0 && oldResolvingShot > 0)
            {
                // Has resolved shot
                m_ShotPower = 0;
            }

            m_NextTurn = m_Turn == Turn::P1 ? Turn::P2 : Turn::P1;
        }

        m_ShootKeyHeldLastFrame = m_ShootKeyHeld;
    }
private:
    int m_StripedBallsRemaining;
    int m_SolidBallsRemaining;
    Turn m_Turn;
    Turn m_TurnLastUpdate;
    Turn m_NextTurn;
    bool m_HasSunkBadly = false;
    float m_ShotPower;
    int m_ShotPowerDirection;

    int m_ProgressBarWidth = 300;
    int m_ProgressBarHeight = 50;
    int m_ProgressBarPadding = 3;

    float m_ResolvingShot;
    bool m_ShootKeyHeld;
    bool m_ShootKeyHeldLastFrame;

    bool m_shotAllowedFlag;
    bool m_ShotWasAllowed = false;
    bool m_shotactivated;
    glm::vec3 m_cueBallPos;
    float m_shotAngle;
    int m_maxShotPower = 400;
    float m_rotateSpeed = 1.1;

    Rigidbody* m_BallRb;
    Transform* m_CueTransform;
    Transform* m_CueBallTransform;
    GameManager* m_Manager;
    Scene* m_Scene;
};