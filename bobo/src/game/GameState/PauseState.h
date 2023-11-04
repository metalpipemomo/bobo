#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include "IGameState.h"

class PauseState : public IGameState
{
public:
    void Enter() override;
    void Exit() override;
    void Pause() override;
    void Resume() override;
    void Update(float dt) override;
    void Render() override;
};

#endif // PAUSESTATE_H
