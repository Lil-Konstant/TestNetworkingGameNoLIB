#include <RakPeerInterface.h>
#include "GameClient.h"

int main()
{
    auto app = new GameClient();
    app->setStartPosition();
    app->run("Networked Pong - Ronan Richardson", 1280, 720, false);
    delete app;

    return 0;
}