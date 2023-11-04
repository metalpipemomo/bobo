#ifndef IGAMESTATE_H
#define IGAMESTATE_H

class IGameState
{
public:
    virtual ~IGameState() = default;

    // Called when the state is entered and becomes the active state.
    virtual void Enter() = 0;

    // Called when the state exits and is no longer the active state.
    virtual void Exit() = 0;

    // Called when the state is paused (e.g., another state is pushed on top of it).
    virtual void Pause() {}

    // Called when the state resumes from being paused (e.g., the state above it is popped).
    virtual void Resume() {}

    // Called every frame to update the state.
    virtual void Update(float dt) = 0;

    // Called every frame to render the state.
    virtual void Render() = 0;
};

#endif // IGAMESTATE_H
