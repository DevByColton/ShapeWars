#include <windows.h>
#include <iostream>
#include "GameRoot.h"

void checkDebug()
{
#ifdef DEBUG_BUILD
    std::cout << "Running in Debug mode." << std::endl;
#elif defined(RELEASE_BUILD)
    FreeConsole();
#else
    std::cout << "Running in an unknown build mode." << std::endl;
#endif
}

int main()
{
    checkDebug();
    GameRoot::instance().load();
    GameRoot::instance().run();
    return 0;
}
