#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include <memory>
#include <stack>
#include "IGameState.h"

class GameStateManager
{
public:
    GameStateManager() = default;
    ~GameStateManager() = default;

    void AddState(std::unique_ptr<IGameState> state, bool isReplacing = true);
    void RemoveState();
    void ProcessStateChanges();

    std::unique_ptr<IGameState>& GetCurrentState();

private:
    std::stack<std::unique_ptr<IGameState>> m_states;
    std::unique_ptr<IGameState> m_newState;
    bool m_isAdding = false;
    bool m_isReplacing = false;
    bool m_isRemoving = false;
};

#endif // GAMESTATEMANAGER_H
