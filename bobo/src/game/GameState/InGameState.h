#ifndef INGAMESTATE_H
#define INGAMESTATE_H

#include "IGameState.h"

class InGameState : public IGameState
{
public:
    void Enter() override;
    void Exit() override;
    void Pause() override;
    void Resume() override;
    void Update(float dt) override;
    void Render() override;
};

#endif // INGAMESTATE_H
