#define SDL_MAIN_HANDLED
#include "Players.hpp"
#include "states.hpp"
#include <iostream>
#include <string>

States state;

int main(int argc, char* argv[]) {
    Players player;

    if (argc > 1){
        std::string arg = argv[1];

        if (arg == "run"){
            player.runMixed();
        }

        if (arg == "river"){
            player.runBoost();
        }

        if (arg == "--version"){
            state.clearScreen();
            std::cout << "\nWASSER AUDIO ENGINE v0.10.0 - SQC Tech\n" << "\n";
        }
    }
    return 0;
}