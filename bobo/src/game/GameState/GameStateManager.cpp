#include "GameStateManager.h"

void GameStateManager::AddState(std::unique_ptr<IGameState> newState, bool isReplacing)
{
    m_isAdding = true;
    m_isReplacing = isReplacing;
    m_newState = std::move(newState);
}

void GameStateManager::RemoveState()
{
    m_isRemoving = true;
}

void GameStateManager::ProcessStateChanges()
{
    if (m_isRemoving && !m_states.empty())
    {
        m_states.top()->Exit();
        m_states.pop();

        if (!m_states.empty())
        {
            m_states.top()->Resume();
        }

        m_isRemoving = false;
    }

    if (m_isAdding)
    {
        if (m_isReplacing && !m_states.empty())
        {
            m_states.top()->Exit();
            m_states.pop();
        }

        if (!m_states.empty())
        {
            m_states.top()->Pause();
        }

        m_states.push(std::move(m_newState));
        m_states.top()->Enter();
        m_isAdding = false;
    }
}

std::unique_ptr<IGameState>& GameStateManager::GetCurrentState()
{
    return m_states.top();
}
