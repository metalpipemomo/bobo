#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include <vector>
#include <memory>
#include "IGameState.h"

class GameStateManager {
private:
    std::vector<std::unique_ptr<IGameState>> states;

public:
    GameStateManager() = default;
    ~GameStateManager();

    // Prevent copy operations
    GameStateManager(const GameStateManager& other) = delete;
    GameStateManager& operator=(const GameStateManager& other) = delete;

    // Allow move operations
    GameStateManager(GameStateManager&& other) noexcept = default;
    GameStateManager& operator=(GameStateManager&& other) noexcept = default;

    void PushState(std::unique_ptr<IGameState> state);
    void PopState();
    void ChangeState(std::unique_ptr<IGameState> state);
    void Update(float deltaTime);
    void Render();
};

#endif // GAMESTATEMANAGER_H