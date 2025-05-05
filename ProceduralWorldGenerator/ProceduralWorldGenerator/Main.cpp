#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Application.h"

#define SDL_MAIN_USE_CALLBACKS 0  /* use the callbacks instead of main() */

enum class AppState
{
    CREATION,
    INIT,
    UPDATE,
    SHUTDOWN,
    EXIT
};


Application* App = nullptr;


static AppState appState;

int SDL_main(int argc, char* argv[])
{

    App = new Application();

    
    AppState appState = AppState::CREATION;
    
    while(appState != AppState::EXIT)
    {
        switch (appState) {
        case AppState::CREATION:

            appState = AppState::INIT;
            break;
        case AppState::INIT:

            App->Init();


            appState = AppState::UPDATE;
            break;
        case AppState::UPDATE:

            App->Update();

            if (App->exit == true)
            {
                appState = AppState::SHUTDOWN;
            }
            break;
        case AppState::SHUTDOWN:

            appState = AppState::EXIT;
            break;

        case AppState::EXIT:

            App->ShutDown();

            break;
        }
    
    }

    return 0;
}