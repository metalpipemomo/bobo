#include "GameState.h"

class InGameState : public GameState
{
public:
    void Enter()
    {
        // Initialize game over resources
        BOBO_INFO("Entered In-Game State");
    }

    void Exit()
    {
        // Clean up game over resources
    }

    void Hold()
    {
        // Pause any game over animations or sounds if necessary
    }

    void Resume()
    {
        // Resume any paused animations or sounds if necessary
    }

    void Update()
    {
        // Update game over logic, such as checking for restart or exit
    }

    void Render()
    {
        // Render the game over screen to the display
    }
};