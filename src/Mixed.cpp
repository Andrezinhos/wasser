#include "Players.hpp" 
#include "Master.hpp" 
#include "states.hpp" 
#include <iostream> 
#include <filesystem>

namespace fs = std::filesystem;

void Players::runMixed(){
    AudioPlayer ap;
    state.clearScreen();
    ap.init();
    while(true){
            std::string title = R"( __  __ _____ ____ ___    _    _  _____ _____ 
|  \/  | ____|  _ \_ _|  / \  | |/ /_ _|_   _|
| |\/| |  _| | | | | |  / _ \ | ' / | |  | |  
| |  | | |___| |_| | | / ___ \| . \ | |  | |  
|_|  |_|_____|____/___/_/   \_\_|\_\___| |_|
Powered by Wasser Mixer
)"; 
        std::cout << title;
        std::cout << "=================================\n";
        std::cout << "enter the commands\n";
        std::cout << "--------------------\n";
        std::cout << "> ";
        std::string cmd;
        std::cin >> cmd;
        if (cmd == "add" || cmd == "+"){
            std::string song;
            std::cout << "Type the music: ";
            std::cin >> song;

            fs::path fullpath = "C:/WasserEngine/songs/" + song;
            if (!fs::exists(fullpath)){
                state.clearScreen();
                std::cout << "Not founded the file\n";
            }
            ap.addToQueue(song);
        }
        else if (cmd == "play" || cmd == "."){
            state.clearScreen();
            ap.play();
            SDL_Delay(100);
        }
        else if (cmd == "pause" || cmd == ";"){
            state.clearScreen();
            ap.pause();
        }
        else if (cmd == "resume" || cmd == ","){
            state.clearScreen();
            ap.resume();
        }
        else if (cmd == "stop" || cmd == "x"){
            state.clearScreen();
            ap.stop();
        }
        else if (cmd == "next" || cmd == "]"){
            state.clearScreen();
            ap.next();
        }
        else if (cmd == "prev" || cmd == "[") {
            state.clearScreen();
            ap.previous();
        }
        else if (cmd == "exit" || cmd == "!"){
            state.clearScreen();
            break;
        }
        else if (cmd == "help"){
            state.clearScreen();
            std::cout << "Commands: add/+ | play/. | pause/; | resume/, | next/] | prev/[ | exit/!\n" << "\n";
            std::cout << "Commands FOR DEVS: DEBUG | NODEBUG \n" << "\n";
        }
        else if (cmd == "DEBUG"){
            state.clearScreen();
            std::cout << "!!! ON DEBUG MODE\n" << "\n";
            state.debugMode = true;
        }
        else if (cmd == "NODEBUG"){
            state.clearScreen();
            std::cout << "!!! OFF DEBUG MODE\n" << "\n";
            state.debugMode = false;
        } 
        else if (cmd == "FADEIN" || cmd == "("){
            state.clearScreen();
            std::cout << "Type the fade time: ";
            int time;
            std::cin >> time;
            ap.fadeIn(time * 1000);
        }
        else if (cmd == "FADEOUT" || cmd == ")"){
            state.clearScreen();
            std::cout << "Type the fade time: ";
            int time;
            std::cin >> time;
            ap.fadeOut(time * 1000);
        }
        else if (cmd == "u"){
            state.clearScreen();
            ap.volUp();
        }
        else if (cmd == "d"){
            state.clearScreen();
            ap.volDown();
        }
        else if (cmd == "vs"){
            state.clearScreen();
            int vol;
            std::cout << "Set the volume: ";
            std::cin >> vol;
            ap.volSet(vol);
        }
        else {
            state.clearScreen();
            std::cout << "Not implemented command\n" << "\n";
        }
        ap.update();
    }
}