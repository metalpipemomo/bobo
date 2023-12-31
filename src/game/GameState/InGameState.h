#include "../../engine/GameState/GameState.h"

#include "../../engine/Notifications/NotificationManager.h"
#include "glm/ext.hpp"
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

        m_FastForwarding = false;
        m_Cue = true;

        m_shotAllowedFlag = false;
        m_ShotWasAllowed = false;
        m_shotactivated = false;
        m_HasSunkBadly = false;

        m_decidedBall = false;
        m_leftSolid = true;

        m_FirstCollisionHit = false;
        m_BallContactFoul = false;
        m_AllBallsStopped = false;
        m_IsCueBallSunk = false;

        m_Turn = Turn::P1;

        if (!m_HasShownHowToPlay)
        {
            m_HasShownHowToPlay = true;
            PopupManager::MakePopup(
                "How to Use your Cue",
                std::vector<PerPopupInformation>
                {
                    PerPopupInformation("Hold Space to Charge your Shot.The Power will\nbounce back and forth between full and empty.",
                        ImVec2(400, 117.5)),
                    PerPopupInformation("Adjust the Cue's angle using 1 and 2", ImVec2(400, 105)),
                    PerPopupInformation("Check the other controls in the pause menu (Escape)", ImVec2(400, 105)),
                    PerPopupInformation("Happy Pooling Gamer!", ImVec2(400, 105))
                },
                AnchorPos::TOP_CENTER,
                ImVec2(0, 25),
                true, true
            );
        }

        m_Scene = SceneManager::GetActiveScene();
        auto objects = m_Scene->GetComponentsOfType<ObjectTag>();
        m_CueBallGhostObject = m_Scene->GetComponentsOfType<CueBallGhost>()[0];

        // getting game object components needed
        for (auto& object : objects)
        {
            m_CueBallGhostObject = m_Scene->GetComponentsOfType<CueBallGhost>()[0];
            if (object->tag == "cue")
            {
                m_CueTransform = m_Scene->GetComponent<Transform>(object->m_OwnerId);
                
            }
            if (object->tag == "cueModel")
            {
                m_CueModelTransform = m_Scene->GetComponent<Transform>(object->m_OwnerId);

            }
            if (object->tag == "cueBall")
            {
                //Cue ball will be placed in its' default location.
                m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->SetMotionType(true);
                m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->SetVelocity({0, 0, 0});
                m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->SetPosition({-0.002514, -1.048805, -2.501616});
                m_BallRb = m_Scene->GetComponent<Rigidbody>(object->m_OwnerId);
                m_CueBallTransform = m_Scene->GetComponent<Transform>(object->m_OwnerId);
                m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->SetMotionType(false);
                
            }
            if (object->tag == "gamemanager")
            {
                //Stripes and solids are set to 7 at the start, which will translate to the game
                //manager.
                m_Manager = m_Scene->GetComponent<GameManager>(object->m_OwnerId);
                m_Manager->stripesAmount = 7;
                m_Manager->solidsAmount = 7;
                
            }
            if (object->tag == "solid" || object->tag == "striped" || object->tag == "8ball")
            {
                m_AllBallsTransforms.push_back(m_Scene->GetComponent<Transform>(object->m_OwnerId));
                m_AllBallsTransformsPrevious.push_back(*m_Scene->GetComponent<Transform>(object->m_OwnerId));
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

            if (object->tag == "frontWallMover")
            {
                m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->SetMotionType(false);
                m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->AddLinearVelocity(JPH::Vec3(0,0,80));
            }

        }

        //Only the balls have been given alt tags, no other entities within the game have them.

        auto objects2 = m_Scene->GetComponentsOfType<ObjectTagAlt>();

        //All the balls will be looped through, and, depending on the ball's number
        //will be placed on a different part of the pool table in the game environment.
        //If the ball was previoiusly disabled, it will be enabled again.
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
            //All the balls have been put into the triangle formation at the start.
            JPH::Vec3 newPos = {x, y, z};
            m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->EnableBody();
            m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->SetPosition(newPos);
            m_Scene->GetComponent<Rigidbody>(object->m_OwnerId)->SetMotionType(false);
        }

        // this was originally just for sound testing but now it works to accompany the
        // walls blowing away 
        auto playAudioBoom = [=]() {
            auto s = Audio::GetSoundInfo("Boom");
            s->SetVolume(.5);
            Audio::PlaySound("Boom");
        };
        auto c = CoroutineScheduler::StartCoroutine<WaitForSeconds>(playAudioBoom, .5);
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
        m_AllBallsStopped = true;
        m_CueModelTransform->rotation = glm::vec3(-0.12, m_shotAngle, 0);
        
        // checks 1 and 2 keys for rotating shot angle; 1 to rotate CCW, 2 to rotate CW
        if (Input::GetKey(GLFW_KEY_1))
        {
            if(Input::GetKey(GLFW_KEY_LEFT_CONTROL))
                m_shotAngle += (m_rotateSpeed/3.0f) * Time::DeltaTime();
            else
                m_shotAngle += m_rotateSpeed * Time::DeltaTime();
        }
        if (Input::GetKey(GLFW_KEY_2))
        {
            if (Input::GetKey(GLFW_KEY_LEFT_CONTROL))
                m_shotAngle -= (m_rotateSpeed/3.0f) * Time::DeltaTime();
            else
                m_shotAngle -= m_rotateSpeed * Time::DeltaTime();
        }

        CheckAllBallsHasStopped();

        // if cue ball almost stopped
        if (m_BallRb->GetVelocity().Length() < m_VelocityThreshold || !m_BallRb->IsEnabled())
        {
            // stop the cue ball
            m_BallRb->SetVelocity(JPH::Vec3{ 0,0,0 });

            // the ghost cue ball (i.e., the cue ball placement mode) is only enabled if all balls have stopped
            if (m_IsCueBallSunk && m_AllBallsStopped)
            {
                m_CueBallGhostObject->Enable();
            }
            
            // if the cue ball has stopped, is not sunk, and the ball was hit
            if (m_BallRb->GetVelocity().Length() == 0 && !m_IsCueBallSunk && m_ResolvingShot < 0) 
            {
                bool foulBall = m_BallContactFoul;
                
                // check if a foul occurred
                if (!foulBall) {
                    foulBall = CheckFoul();
                }

                // if a foul occurred, send notifications, disable the cueball, and enable the cueball placement mode
                if (foulBall) 
                {
                    m_CueBallTransform->position = glm::vec3{ 100,100,100 };
                    m_BallRb->DisableBody();
                    NotificationManager::SendSlidingBannerNotification("Foul: Player either failed to hit a ball or failed to hit a ball of their type!", 
                        NotificationTextColor::RED);
                    NotificationManager::SendSlidingBannerNotification("Re-place the cue ball!", NotificationTextColor::WHITE);
                    m_CueBallGhostObject->Enable();
                }
                m_FirstCollisionHit = false;
                m_ResolvingShot = 0;
            }

            // set cue shot indicator and set flag to true
            if (m_AllBallsStopped)
            {
                m_shotAllowedFlag = true;
                m_CueTransform->position = m_CueBallTransform->position + glm::vec3(Sin(m_shotAngle) * -1, 0, Cos(m_shotAngle) * -1);
                m_CueModelTransform->position = m_CueBallTransform->position + 
                glm::vec3(Sin(m_shotAngle)*(3+m_ShotPower), 0.3 + (0.1*m_ShotPower), Cos(m_shotAngle) * (3 + m_ShotPower));
            }
        }

        // handle events related to when the cueball has actually been hit
        if (m_shotactivated && m_shotAllowedFlag)
        {
            // reset variables to false to make a clean slate for a new turn
            m_IsCueBallSunk = false;
            m_shotAllowedFlag = false;
            m_shotactivated = false;
            m_ShotWasAllowed = false;
            m_HasSunkBadly = false;
            m_CueTransform->position = glm::vec3{ 100,100,100 };

            // make cue model dissapear after some time to make it look like a real shot
            m_CueModelTransform->position = m_CueBallTransform->position + glm::vec3(Sin(m_shotAngle) * 3, 0.3, Cos(m_shotAngle) * 3);
            float waitTime = 0.5;
            auto makeCueDissapear = [=]() {
                m_CueModelTransform->position = glm::vec3{ 100,100,100 };
                };
            auto c = CoroutineScheduler::StartCoroutine<WaitForSeconds>(makeCueDissapear, waitTime);
            
            // play a sound for when the cue is hit
            if (m_Cue) {
                auto s = Audio::GetSoundInfo("CueHit");
                s->SetVolume(m_ShotPower / m_maxShotPower * 100);
                Audio::PlaySound("CueHit");
            }
            m_Cue = !m_Cue;
            
            // make the cueball move
            m_BallRb->addForce(m_PlayerMuscles * JPH::Vec3(Sin(m_shotAngle) * -m_ShotPower * m_maxShotPower, 0, Cos(m_shotAngle) * -m_ShotPower * m_maxShotPower));
        }
    }

    void Update()
    {
        // do game updating stuff
        UpdateGame();

        // if the turns have changed
        if (m_Turn != m_TurnLastUpdate)
        {
            // make a banner declaring this new turn
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
        // keep track of what the turn just was
        m_TurnLastUpdate = m_Turn;

        // when everything has settled and the cueball is once again ready to be hit, the turn will be resolved
        // based on the results of the prior turn (if the player sunk a ball of their type w/o sinking the other person's or not)
        if (!m_ShotWasAllowed && m_shotAllowedFlag) {
            m_Turn = m_NextTurn;
            m_HasSunkBadly = false;
            m_ShotWasAllowed = true;
        }
    }

    // checks if all balls have stopped, or should stop; if a ball is just barely moving, it will be stopped manually
    void CheckAllBallsHasStopped()
    {
        // check if all balls are stopped
        for (auto& ball : m_AllBallsTransforms)
        {
            auto ballRb = m_Scene->GetComponent<Rigidbody>(ball->m_OwnerId);
            // if a ball is almost stopped or is not enabled
            if (ballRb->GetVelocity().Length() < m_VelocityThreshold || !ballRb->IsEnabled())
            {
                // stop the ball
                ballRb->SetVelocity(JPH::Vec3{ 0,0,0 });
            }
            else
            {
                m_AllBallsStopped = false;
                break;
            }
        }

    }

    bool compareVec3(const glm::vec3& v1, const glm::vec3& v2, double epsilon) 
    {
        double diffX = glm::abs(v1.x - v2.x);
        double diffY = glm::abs(v1.y - v2.y);
        double diffZ = glm::abs(v1.z - v2.z);
        // Check if all differences are within the epsilon or very small difference
        return (diffX <= epsilon) && (diffY <= epsilon) && (diffZ <= epsilon);
    }

    void applyBallGroupCollisionTrigger()
    {
        if (m_decidedBall) {
            return;
        }
  
        // on the break make sure first collision is set true
        m_FirstCollisionHit = true;

        m_BallRb->SetOnCollision([=](JPH::BodyID other) {
            //once the first collision is accounted for, don't bother calculating
            if (m_FirstCollisionHit)
            {
                return;
            }
            Entity en = Physics::GetInstance()->GetEntityFromJoltRb(other);
            auto otherObject = m_Scene->GetComponent<ObjectTag>(en);
            if (otherObject != NULL) {
                // Check the type of the collided object
                if (otherObject->tag == "solid" || otherObject->tag == "striped") {
                    bool isCollidedBallSolid = (otherObject->tag == "solid");
                    bool ballNotFromCurrentPlayerGroup;
                    // Check if the collided ball doesn't belong to the current player's group based on m_Turn
                    if (m_leftSolid)
                    {
                        ballNotFromCurrentPlayerGroup = ((m_Turn == Turn::P1 && !isCollidedBallSolid) || (m_Turn == Turn::P2 && isCollidedBallSolid));
                    }
                    else
                    {
                        ballNotFromCurrentPlayerGroup = ((m_Turn == Turn::P1 && isCollidedBallSolid) || (m_Turn == Turn::P2 && !isCollidedBallSolid));
                    }

                    // Set m_BallContactFoul based on whether the collided ball doesn't belong to the current player's group
                    m_BallContactFoul = ballNotFromCurrentPlayerGroup;
                    m_FirstCollisionHit = true;
                }
                else if (otherObject->tag == "8ball") {
                    // If it hit an 8-ball first, it's an automatic foul
                    m_BallContactFoul = true;
                    m_FirstCollisionHit = true;
                }
                else {
                    // Default case
                    m_BallContactFoul = false;
                    m_FirstCollisionHit = false;
                }
            }
        });
    }

    bool CheckFoul() {
        auto objects = m_Scene->GetComponentsOfType<ObjectTag>();
        m_AllBallsTransformsUpdated.clear();
      
        // Gather updated ball transforms
        for (auto& object : objects) 
        {
            if (object->tag == "solid" || object->tag == "striped" || object->tag == "8ball") 
            {
                m_AllBallsTransformsUpdated.push_back(*m_Scene->GetComponent<Transform>(object->m_OwnerId));
            }
        }

        if (m_AllBallsTransformsPrevious.size() == m_AllBallsTransformsUpdated.size()) 
        {
            bool anyDifferent = false; // Flag to track if any ball is markedly different

            // Check for position differences
            for (size_t i = 0; i < m_AllBallsTransformsPrevious.size(); ++i) 
            {
                Transform initial = m_AllBallsTransformsPrevious[i];
                Transform updated = m_AllBallsTransformsUpdated[i];
               
                // If any ball is markedly different, set the flag and break the loop
                if (!compareVec3(initial.position, updated.position, 0.001)) 
                {
                    anyDifferent = true;
                    break;
                }
            }

            // If all balls are essentially at the same spot, consider it a foul
            if (!anyDifferent) 
            {
                m_AllBallsTransformsPrevious = m_AllBallsTransformsUpdated;
                return true;
            }
        }
        else 
        {
            // Ball transform counts don't match, return false
            m_AllBallsTransformsPrevious = m_AllBallsTransformsUpdated;
            return false;
        }

        // No foul detected, return false
        m_AllBallsTransformsPrevious = m_AllBallsTransformsUpdated;
        return false;
    }

    void SinkSolid() 
    {
        NotificationManager::SendDefaultNotification("A Solid Ball has been Sunk", NotificationTextColor::BLUE);
        
        // if it's player 1's turn
        if (m_Turn == Turn::P1)
        {
            // if the ball hasn't been decided, their ball will become solid
            if (!m_decidedBall) {
                m_leftSolid = true;
                // and they get another turn if the cueball isn't sunk
                if (!m_IsCueBallSunk)
                    m_NextTurn = Turn::P1;
            }
            // if p1 is not solid, they've sunk badly, so next turn will be p2's turn
            else if (!m_leftSolid) {
                m_NextTurn = Turn::P2;
                m_HasSunkBadly = true;
            }
            // if p1 is solid and hasn't sunk badly already, the next turn might just be theirs...!!!!!
            else if (!m_HasSunkBadly)
                m_NextTurn = Turn::P1;
        }
        // essentially the same as the prior but for p2;
        // if ball hasn't been determined, p2 becomes solid and if the cueball hasn't sunk they get another turn
        // if they're not solid, they've sunk badly, so next turn will be p1's turn
        // if they haven't sunk badly, they will get another turn unless they sink badly later
        if (m_Turn == Turn::P2) 
        {
            if (!m_decidedBall){
                m_leftSolid = false;
                if (!m_IsCueBallSunk)
                    m_NextTurn = Turn::P2;
            }
            else if (m_leftSolid) {
                m_NextTurn = Turn::P1;
                m_HasSunkBadly = true;
            }
            else if(!m_HasSunkBadly)
                m_NextTurn = Turn::P2;
        }
        applyBallGroupCollisionTrigger();
        m_decidedBall = true;
    }

    // this function is essentially the same as SinkSolid, but for sinking a striped
    // to summarise:
    // - if the ball type hasn't been decided, whoever sunk will become striped,
    //   and get another turn (if they haven't sunk the cueball)
    // - if the person sunk a ball that was not theirs, they have sunk badly, so the
    //   next turn will be their opponent's no matter what
    // - if the person sunk a ball that was theirs and have not sunk badly, the next
    //   turn will become theirs unless they later sink a ball that was not theirs 
    void SinkStriped() 
    {
        NotificationManager::SendDefaultNotification("A Striped Ball has been Sunk", NotificationTextColor::RED);
        
        if (m_Turn == Turn::P1)
        {
            if (!m_decidedBall) {
                m_leftSolid = false;
                if (!m_IsCueBallSunk) 
                    m_NextTurn = Turn::P1;
            }
            else if (m_leftSolid) {
                m_NextTurn = Turn::P2;
                m_HasSunkBadly = true;
            }
            else if (!m_HasSunkBadly)
                m_NextTurn = Turn::P1;
        }
        if (m_Turn == Turn::P2)
        {
            if (!m_decidedBall) {
                m_leftSolid = true;
                if (!m_IsCueBallSunk)
                    m_NextTurn = Turn::P2;
            }
            else if (!m_leftSolid) {
                m_NextTurn = Turn::P1;
                m_HasSunkBadly = true;
            }
            else if (!m_HasSunkBadly)
                m_NextTurn = Turn::P2;
        }
        applyBallGroupCollisionTrigger();
        m_decidedBall = true;
        
    }

    // ends the game and sets the winner as whoever won
    void SetWinner(std::string winner, std::string winReason) 
    {
        GameOverState* gameOverState = (GameOverState*)GameStateManager::FetchGameState(GameStateLabel::GAME_OVER);
        gameOverState->SetWinner(winner, winReason);
        GameStateManager::EnterGameState(GameStateLabel::GAME_OVER);
    }

    // handles ending the game; game only ends when the 8ball is sunk
    void Sink8Ball()
    {
        // if there's still balls remaining, then whoever sunk the ball lost
        if (m_SolidBallsRemaining > 0 && m_StripedBallsRemaining > 0) {
            if (m_Turn == Turn::P1)
                SetWinner("Player 2", "Player 1 sunk the 8 ball before sinking all of their other balls.");
            else
                SetWinner("Player 1", "Player 2 sunk the 8 ball before sinking all of their other balls.");

            return;
        }
        // if player 1 is solid
        if (m_leftSolid) {
            // player 1 wins if there's no solid balls left
            if (m_SolidBallsRemaining <= 0)
                SetWinner("Player 1", "Player 1 has sunk all their balls and the 8 ball.");
            // otherwise, player 2 wins
            else
                SetWinner("Player 2", "Player 2 has sunk all their balls and the 8 ball.");
            return;
        }
        // otherwise, player 1 wins if there's no striped balls left
        if (m_StripedBallsRemaining <= 0)
            SetWinner("Player 1", "Player 1 has sunk all their balls and the 8 ball.");
        // otherwise, player 2 wins
        else
            SetWinner("Player 2", "Player 2 has sunk all their balls and the 8 ball.");
    }

    void SinkCueBall()
    {
        // if the player sinks the cue ball, they've sunk badly,
        // and it should become the other player's turn
        m_IsCueBallSunk = true;
        m_HasSunkBadly = true;
        if (m_Turn == Turn::P1)
        {
            m_NextTurn = Turn::P2;
        }
        else if (m_Turn == Turn::P2)
        {
            m_NextTurn = Turn::P1;
        }
    }

    // this function creates and handles the majority of UI elements in the ingamestate
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
        if (!m_decidedBall)
            ImGuiHelpers::MakeCenterText("??? Remaining: 7");
        else if (m_leftSolid)
            ImGuiHelpers::MakeCenterText("Solid Balls Remaining: " + std::to_string(m_SolidBallsRemaining));
        else
            ImGuiHelpers::MakeCenterText("Striped Balls Remaining: " + std::to_string(m_StripedBallsRemaining));
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
        if (!m_decidedBall)
            ImGuiHelpers::MakeCenterText("??? Remaining: 7");
        else if (m_leftSolid)
            ImGuiHelpers::MakeCenterText("Striped Balls Remaining: " + std::to_string(m_StripedBallsRemaining));
        else
            ImGuiHelpers::MakeCenterText("Solid Balls Remaining: " + std::to_string(m_SolidBallsRemaining));
        ImGuiHelpers::LowerCursor();

        ImGui::End();

    
        // Controls
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + WINDOW_WIDTH - 250, main_viewport->WorkPos.y + 150), 0);
        ImGui::SetNextWindowSize(ImVec2(225, 325), 0);

        ImGui::StyleColorsDark();

        ImGui::Begin("Controls", NULL, ImGuiHelpers::MakeFlags(false, true, true, true, true, true, true, false, false, false));

        if  (Camera::GetMode() == 0) {
            ImGuiHelpers::LowerCursor();

            ImGuiHelpers::MakeVerticalList(
            {
                "Enable Free Cam - Ctrl + Tab",
                "Exit Free Cam - Tab",
                "Move - WASD",
                "Look - Arrow Keys",
                "Move Up - Ctrl + W",
                "Move Down - Ctrl + D"
            }, 10, "Free Camera");
        } else if (m_CueBallGhostObject->enabled) {
            ImGuiHelpers::LowerCursor();

            ImGuiHelpers::MakeVerticalList(
            {
                "Move F/B - W <-> S", 
                "Move L/R - A <-> D",
                "Confirm Placement - P",
            }, 10, "Re-placing Cue Ball");
        } else if (!m_CueBallGhostObject->enabled) {
            ImGuiHelpers::LowerCursor();

            ImGuiHelpers::MakeVerticalList(
            {
                "Rotate Cue L/R - 1 <-> 2",
                "Fine Adjust - Ctrl + 1 <-> 2",
                "Fast Forward - F",
                "Charge Shot - Hold Space"
            }, 10, "Gameplay");

            ImGuiHelpers::LowerCursor();

            ImGuiHelpers::MakeVerticalList(
            {
                "Rotate Camera L/R - A <-> D",
                "Zoom Camera - W <-> S",
                "Toggle Camera Mode - Tab",
                "Enable Free Cam - Ctrl + Tab"
            }, 10, "Game Camera");
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

        // Time Control
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + WINDOW_WIDTH - 225, main_viewport->WorkPos.y + WINDOW_HEIGHT - 135), 0);
        ImGui::SetNextWindowSize(ImVec2(200, 75), 0);

        ImGui::Begin("Time Control", NULL, ImGuiHelpers::MakeFlags(true, true, true, true, true, true, true, false, false, false));

        ImGuiHelpers::LowerCursor();
        if (ImGuiHelpers::MakeCenterButton("Toggle Fast Forward", true, false) || Input::GetKeyDown(GLFW_KEY_F)) {
            m_FastForwarding = !m_FastForwarding;
            if (m_FastForwarding)
                Time::SetTimeScale(2);
            else
                Time::SetTimeScale(1);
            NotificationManager::SendDefaultNotification("Time Scale is now " + std::to_string(Time::GetTimeScale()),
                NotificationTextColor::WHITE);
        }

        ImGuiHelpers::LowerCursor();
        std::string ffString = m_FastForwarding ? "Enabled" : "Disabled";
        ImGuiHelpers::MakeCenterText("Fast Forwarding " + ffString, true, false);

        ImGui::End();
    }

    virtual void Resume()
    {
        if (m_FastForwarding)
            Time::SetTimeScale(2);
        else
            Time::SetTimeScale(1);
    }

private:
    // References
    Rigidbody* m_BallRb;
    Transform* m_CueTransform;
    Transform* m_CueModelTransform;
    Transform* m_CueBallTransform;
    GameManager* m_Manager;
    Scene* m_Scene;
    CueBallGhost* m_CueBallGhostObject;
    std::vector<Transform*> m_AllBallsTransforms;
    std::vector<Transform> m_AllBallsTransformsPrevious;
    std::vector<Transform> m_AllBallsTransformsUpdated;

    // Game tracking
    int m_StripedBallsRemaining;
    int m_SolidBallsRemaining;
    Turn m_Turn;
    Turn m_TurnLastUpdate;
    Turn m_NextTurn;
    bool m_Cue = true;

    float m_ShotPower;
    int m_ShotPowerDirection;

    float m_shotAngle;

    glm::vec3 m_cueBallPos;

    bool m_shotAllowedFlag = false;
    bool m_ShotWasAllowed = false;
    bool m_shotactivated = false;
    bool m_HasSunkBadly = false;

    // Deciding which ball type belongs to each player
    bool m_decidedBall = false;
    bool m_leftSolid = true;

    // Fouls & other
    bool m_FirstCollisionHit = false;
    bool m_BallContactFoul = false;
    bool m_AllBallsStopped = false;
    bool m_IsCueBallSunk = false;

    // Adjustables
    int m_maxShotPower = 750;
    float m_rotateSpeed = 1.1;
    float m_PlayerMuscles = 1.25;
    float m_VelocityThreshold = 0.2;

    // UI
    int m_ProgressBarWidth = 300;
    int m_ProgressBarHeight = 50;
    int m_ProgressBarPadding = 3;
    float m_ResolvingShot;
    bool m_ShootKeyHeld;
    bool m_ShootKeyHeldLastFrame;

    // Other
    bool m_FastForwarding = false;
    bool m_HasShownHowToPlay = false;
};