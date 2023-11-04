#include "GameStateManager.h"

GameStateManager::~GameStateManager() {
    while (!states.empty()) {
        states.back()->Exit();
        states.pop_back();
    }
}

void GameStateManager::PushState(std::unique_ptr<IGameState> state) {
    if (!states.empty()) {
        states.back()->Exit();
    }
    states.push_back(std::move(state));
    states.back()->Enter();
}

void GameStateManager::PopState() {
    if (!states.empty()) {
        states.back()->Exit();
        states.pop_back();
    }
    if (!states.empty()) {
        states.back()->Enter();
    }
}

void GameStateManager::ChangeState(std::unique_ptr<IGameState> state) {
    while (!states.empty()) {
        states.back()->Exit();
        states.pop_back();
    }
    states.push_back(std::move(state));
    states.back()->Enter();
}

void GameStateManager::Update(float deltaTime) {
    if (!states.empty()) {
        states.back()->Update(deltaTime);
    }
}

void GameStateManager::Render() {
    if (!states.empty()) {
        states.back()->Render();
    }
}