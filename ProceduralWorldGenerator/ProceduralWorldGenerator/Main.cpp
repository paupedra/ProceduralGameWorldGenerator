#define SDL_MAIN_HANDLED
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

Application* Application::instance = nullptr;

static AppState appState;

int SDL_main(int argc, char* argv[])
{
    
    AppState appState = AppState::CREATION;
    
    while(appState != AppState::EXIT)
    {
        switch (appState) {
        case AppState::CREATION:

            appState = AppState::INIT;
            break;
        case AppState::INIT:

            if (!Application::get().Init())
            {
                appState = AppState::EXIT;
            }


            appState = AppState::UPDATE;
            break;
        case AppState::UPDATE:

            Application::get().Update();

            if (Application::get().exit == true)
            {
                appState = AppState::SHUTDOWN;
            }
            break;
        case AppState::SHUTDOWN:

            appState = AppState::EXIT;
            break;

        case AppState::EXIT:

            Application::get().ShutDown();
            Application::destruct();
            break;
        }
    
    }

    return 0;
}