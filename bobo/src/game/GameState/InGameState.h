#ifndef INGAMESTATE_H
#define INGAMESTATE_H

#include "IGameState.h"

class InGameState : public IGameState
{
public:
    void Enter() override;
    void Exit() override;
    void Update(float deltaTime) override;
    void Render() override;
};

#endif // INGAMESTATE_H
