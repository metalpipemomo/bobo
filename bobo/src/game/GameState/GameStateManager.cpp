#include "GameStateManager.h"

GameStateManager::~GameStateManager()
{
    while (!m_States.empty())
    {
        m_States.back()->Exit();
        m_States.pop_back();
    }
}

void GameStateManager::PushState(std::unique_ptr<IGameState> pState)
{
    if (!m_States.empty())
    {
        m_States.back()->Exit();
    }
    m_States.push_back(std::move(pState));
    m_States.back()->Enter();
}

void GameStateManager::PopState()
{
    if (!m_States.empty())
    {
        m_States.back()->Exit();
        m_States.pop_back();
    }
    if (!m_States.empty())
    {
        m_States.back()->Enter();
    }
}

void GameStateManager::ChangeState(std::unique_ptr<IGameState> pState)
{
    while (!m_States.empty())
    {
        m_States.back()->Exit();
        m_States.pop_back();
    }
    m_States.push_back(std::move(pState));
    m_States.back()->Enter();
}

void GameStateManager::Update(float deltaTime)
{
    if (!m_States.empty())
    {
        m_States.back()->Update(deltaTime);
    }
}

void GameStateManager::Render()
{
    if (!m_States.empty())
    {
        m_States.back()->Render();
    }
}