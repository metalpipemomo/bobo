#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "IGameState.h"

class MainMenuState : public IGameState
{
public:
    void Enter() override;
    void Exit() override;
    void Update(float deltaTime) override;
    void Render() override;
};

#endif // MAINMENUSTATE_H