#include "game.h"
#include "model.h"

// Using tutorial at https://www.youtube.com/watch?v=iYZA1k8IKgM&list=PL6xSOsbVA1eYSZTKBxnoXYboy7wc4yg-Z&index=9

int main() 
{
    Game game("YOUTUBE_TUTORIAL", 1920, 1080, 4, 5, false);

    // MAIN LOOP
    while (!game.getWindowShouldClose()) // while the window is not being closed
    {
        // Update input
        game.update();
        game.render();
    }

    return 0;
}