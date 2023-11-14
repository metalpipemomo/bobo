#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include <memory>
#include <stack>
#include "IGameState.h"

class GameStateManager
{
public:
    static void GameStateManager::AddState(std::unique_ptr<IGameState> newState, bool isReplacing)
    {
        auto gs = GetInstance();
        gs->m_isAdding = true;
        gs->m_isReplacing = isReplacing;
        gs->m_newState = std::move(newState);
    }

    static void GameStateManager::RemoveState()
    {
        auto gs = GetInstance();
        gs->m_isRemoving = true;
    }

    static void GameStateManager::ProcessStateChanges()
    {
        auto gs = GetInstance();
        if (gs->m_isRemoving && !gs->m_states.empty())
        {
            gs->m_states.top()->Exit();
            gs->m_states.pop();

            if (!gs->m_states.empty())
            {
                gs->m_states.top()->Resume();
            }

            gs->m_isRemoving = false;
        }

        if (gs->m_isAdding)
        {
            if (gs->m_isReplacing && !gs->m_states.empty())
            {
                gs->m_states.top()->Exit();
                gs->m_states.pop();
            }

            if (!gs->m_states.empty())
            {
                gs->m_states.top()->Pause();
            }

            gs->m_states.push(std::move(gs->m_newState));
            gs->m_states.top()->Enter();
            gs->m_isAdding = false;
        }
    }

    static std::unique_ptr<IGameState>& GameStateManager::GetCurrentState()
    {
        auto gs = GetInstance();
        return gs->m_states.top();
    }

private:
    static GameStateManager* GetInstance()
    {
        static GameStateManager* instance = new GameStateManager();
        return instance;
    }
    GameStateManager() {}
    std::stack<std::unique_ptr<IGameState>> m_states;
    std::unique_ptr<IGameState> m_newState;
    bool m_isAdding = false;
    bool m_isReplacing = false;
    bool m_isRemoving = false;
};

#endif // GAMESTATEMANAGER_H
