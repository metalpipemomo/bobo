#pragma once

#include <stack>
#include <map>

#include "GameState.h"

enum GameStateLabel { GAME_OVER, IN_GAME, MAIN_MENU };

class GameStateManager
{
public:
    static void InitWithGameStates(std::vector<std::pair<GameStateLabel, GameState*>> states, GameState* pauseState)
    {
        auto gs = GetInstance();
        for (auto& state : states)
        {
            gs->RegisterGameState(state.first, state.second);
        }
        gs->RegisterPauseState(pauseState);
    }

    static void ResetGameStateStack()
    {
        auto gs = GetInstance();
        while (!gs->m_GameStates.empty())
        {
            gs->ExitCurrentState();
        }
    }

    static GameState* FetchGameState(GameStateLabel gameStateLabel)
    {
        auto gs = GetInstance();
        if (!gs->DoesGameStateExist(gameStateLabel)) return nullptr;
        return gs->m_RegisteredGameStates[gameStateLabel];
    }

    static void EnterGameState(GameStateLabel newGameState, bool exitPrevious = true)
    {
        auto gs = GetInstance();

        // Verify game state exists
        if (!gs->DoesGameStateExist(newGameState)) return;

        if (exitPrevious)
        {
            GameState* prevState = gs->GetCurrentState();
            if (prevState != nullptr)
            {
                gs->ExitCurrentState();
            }
        }
        
        // Enter
        gs->EnterState(gs->m_RegisteredGameStates[newGameState]);
    }

    static void TogglePauseState()
    {
        auto gs = GetInstance();
        if (GetCurrentState() == gs->p_PauseState)
        {
            // Unpause
            gs->ExitCurrentState();
            gs->ResumeCurrentState();
        }
        else if (GetCurrentState() == gs->m_RegisteredGameStates[GameStateLabel::IN_GAME])
        {
            // Pause
            gs->HoldCurrentState();
            gs->EnterState(gs->p_PauseState);
        }
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
        if (gs->m_GameStates.empty()) return nullptr;
        return gs->m_GameStates.top();
    }

    void RegisterGameState(GameStateLabel gameStateLabel, GameState* gameState)
    {
        auto gs = GetInstance();

        // verify game state does not already exist
        if (gs->DoesGameStateExist(gameStateLabel))
        {
            return;
        }

        // register game state
        gs->m_RegisteredGameStates[gameStateLabel] = gameState;
    }

    void RegisterPauseState(GameState* pauseState)
    {
        auto gs = GetInstance();
        gs->p_PauseState = pauseState;
    }

    bool DoesGameStateExist(GameStateLabel gameStateLabel)
    {
        return m_RegisteredGameStates.find(gameStateLabel) != m_RegisteredGameStates.end();
    }
private:
    static GameStateManager* GetInstance()
    {
        static GameStateManager* instance = new GameStateManager();
        return instance;
    }

    void HoldCurrentState()
    {
        m_GameStates.top()->Hold();
    }

    void ResumeCurrentState()
    {
        m_GameStates.top()->Resume();
    }

    void ExitCurrentState()
    {
        m_GameStates.top()->Exit();
        m_GameStates.pop();
    }

    void EnterState(GameState* state)
    {
        m_GameStates.push(state);
        GetCurrentState()->Enter();
    }
    GameStateManager() {}

    std::map<GameStateLabel, GameState*> m_RegisteredGameStates;
    std::stack<GameState*> m_GameStates;
    GameState* p_PauseState;
};