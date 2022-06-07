#include "GameClient.h"

/// <summary>
/// main() simply instantiates a new GameClient object and calls run on it.
/// This object is then deleted once the gameplay loop has exited.
/// </summary>
int main()
{
    auto app = new GameClient();
    app->run("Test Application - Ronan Richardson", 1000, 1000, false);
    delete app;

    return 0;
}