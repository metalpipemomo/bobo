#pragma once

class GameState
{
public:
    GameState() = default;
    virtual ~GameState() = default;

    // Called when the state is entered and becomes the active state.
    virtual void Enter() {};

    // Called when the state exits and is no longer the active state.
    virtual void Exit() {};

    // Called every frame to update the state.
    virtual void Update() {};

    // Called every frame to render the state.
    virtual void Render() {};

    // Called when the state is paused
    virtual void Hold() {}

    // Called when the state resumes from being paused
    virtual void Resume() {}
};