#pragma once

#include "../../bpch.h"

class GameState
{
public:
    GameState() = default;
    virtual ~GameState() = default;

    // Called when the state is entered and becomes the active state
    virtual void Enter() {};

    // Called when the state exits and is no longer the active state
    virtual void Exit() {};

    // Called every frame to update the state
    virtual void Update() {};

    // Called every frame to render the state.
    virtual void Render() {};

    // Called when being superceeded by the Pause State
    virtual void Hold() {};

    // Called when being Pause State is exiting into this State
    virtual void Resume() {};
};