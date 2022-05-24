#include <RakPeerInterface.h>
#include "GameClient.h"

int main()
{
    auto app = new GameClient();
    app->run("Networked Pong - Ronan Richardson", 1000, 1000, false);
    delete app;

    return 0;
}