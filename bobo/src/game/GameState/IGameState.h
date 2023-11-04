#ifndef IGAMESTATE_H
#define IGAMESTATE_H

class IGameState {
public:
    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual ~IGameState() = default;
};

#endif // IGAMESTATE_H