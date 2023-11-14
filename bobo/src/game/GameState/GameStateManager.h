#include <stack>
#include <map>

#include "GameState.h"

#include "GameOverState.h"
#include "InGameState.h"
#include "MainMenuState.h"
#include "PauseMenuState.h"

enum GameStateLabel { GAME_OVER, IN_GAME, MAIN_MENU, PAUSE_MENU };

class GameStateManager
{
public:
    static void Init()
    {
        auto gs = GetInstance();
        gs->RegisterGameState(GameStateLabel::GAME_OVER, new GameOverState());
        gs->RegisterGameState(GameStateLabel::IN_GAME, new InGameState());
        gs->RegisterGameState(GameStateLabel::MAIN_MENU, new MainMenuState());
        gs->RegisterGameState(GameStateLabel::PAUSE_MENU, new PauseMenuState());
    }

    static void ResetGameStateStack()
    {
        auto gs = GetInstance();

        // Pop from the stack until it is empty
        while (!gs->m_GameStates.empty())
            gs->m_GameStates.pop();
    }

    static void EnterGameState(GameStateLabel newGameState)
    {
        auto gs = GetInstance();

        // verify game state exists
        if (!gs->DoesGameStateExist(newGameState)) return;

        // Hold previous game state
        if (gs->HasGameStates())
            GetCurrentState()->Hold();

        // Enter
        gs->m_GameStates.push(gs->m_RegisteredGameStates[newGameState]);
        gs->GetCurrentState()->Enter();
    }

    static void PopGameState()
    {
        auto gs = GetInstance();

        if (gs->m_GameStates.empty()) return;

        // Exit
        gs->GetCurrentState()->Exit();
        gs->m_GameStates.pop();

        // Resume previous game state
        if (gs->HasGameStates())
            GetCurrentState()->Resume();
    }

    static void UpdateCurrentGameState()
    {
        GameState* currentState = GetCurrentState();
        if (currentState == nullptr) return;
        currentState->Update();
        currentState->Render();
    }

    static GameState* GetCurrentState()
    {
        auto gs = GetInstance();
        return gs->m_GameStates.top();
    }

    bool DoesGameStateExist(GameStateLabel gameStateLabel)
    {
        return m_RegisteredGameStates.find(gameStateLabel) != m_RegisteredGameStates.end();
    }

    bool HasGameStates()
    {
        return !m_GameStates.empty();
    }
private:
    static GameStateManager* GetInstance()
    {
        static GameStateManager* instance = new GameStateManager();
        return instance;
    }
    GameStateManager() {}

    void RegisterGameState(GameStateLabel gameStateLabel, GameState* gameState)
    {
        auto gs = GetInstance();

        // verify game state does not already exist
        if (gs->DoesGameStateExist(gameStateLabel))
        {
            BOBO_ERROR("Attempt to register duplicate game state blocked");
            return;
        }

        // register game state
        gs->m_RegisteredGameStates[gameStateLabel] = gameState;
    }

    std::map<GameStateLabel, GameState*> m_RegisteredGameStates;
    std::stack<GameState*> m_GameStates;
};