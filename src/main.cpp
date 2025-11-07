#include "GameRoot.h"

int main()
{
    GameRoot::instance().load();
    GameRoot::instance().run();
    return 0;
}
