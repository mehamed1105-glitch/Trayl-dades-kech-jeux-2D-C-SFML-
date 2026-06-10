#include "Game.h"
#include <iostream>

int main()
{
    try {
        Game game;
        game.run();
    }
    catch (const std::exception& ex) {
        std::cerr << "[ERREUR] " << ex.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}