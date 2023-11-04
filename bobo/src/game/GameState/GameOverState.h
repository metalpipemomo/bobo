#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "IGameState.h"

class GameOverState : public IGameState
{
public:
    void Enter() override;
    void Exit() override;
    void Update(float deltaTime) override;
    void Render() override;
};

#endif // GAMEOVERSTATE_H
