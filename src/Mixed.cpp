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
Powered by Wasser
)"; 
        std::cout << title << "\n";
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

            fs::path fullpath = "C:/Wasser/bin/songs/" + song;
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
        else if (cmd == "play -loop" || cmd == ".,"){
            state.clearScreen();
            state.OnLoop = true;
            ap.play();
            SDL_Delay(100);
        }
        else if (cmd == "playch" || cmd == ".."){
            state.clearScreen();
            std::string sound;
            std::cout << "Type the music: ";
            std::cin >> sound;

            int channel;
            std::cout << "Channel number (-1 for auto): ";
            std::cin >> channel;

            fs::path fullpath = "C:/Wasser/bin/songs/" + sound;
            if (!fs::exists(fullpath)){
                state.clearScreen();
                std::cout << "Not founded the file\n";
            }
            ap.multiPlay(fullpath.string(), channel);
        }
        else if (cmd == "pause" || cmd == ";"){
            state.clearScreen();
            state.OnLoop = false;
            ap.pause();
        }
        else if (cmd == "pausech" || cmd == ";;"){
            state.clearScreen();
            int channel;
            std::cout << "Channel: ";
            std::cin >> channel;
            ap.pauseChannel(channel);
        }
        else if (cmd == "resume" || cmd == ","){
            state.clearScreen();
            ap.resume();
        }
        else if (cmd == "resumech" || cmd == ",,"){
            state.clearScreen();
            int channel;
            std::cout << "Channel: ";
            std::cin >> channel;
            ap.resumeChannel(channel);
        }
        else if (cmd == "stop" || cmd == "x"){
            state.clearScreen();
            ap.stop();
        }
        else if (cmd == "stopch" || cmd == "xx"){
            state.clearScreen();
            int channel;
            std::cout << "Channel: ";
            std::cin >> channel;
            ap.stopChannel(channel);
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
            std::cout << "Commands: add/+ | play/. | pause/; | resume/, | next/] | prev/[ | exit/!" << "\n";
            std::cout << "Channels Commands: playch/.. | resumech/,, | pausech/;; | stopch/xx\n";
            std::cout << "\nCommands FOR DEVS: DEBUG | NODEBUG \n" << "\n";
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
        else if (cmd == "vs"){
            state.clearScreen();
            int vol;
            std::cout << "Set the volume: ";
            std::cin >> vol;
            ap.volSet(vol);
        }
        else if (cmd == "vsch"){
            int channel, vol;
            std::cout << "Channel: ";
            std::cin >> channel;
            std::cout << "Vol: ";
            std::cin >> vol;
            ap.setChannelVol(channel, vol);
        }
        else {
            state.clearScreen();
            std::cout << "Not implemented command\n" << "\n";
        }
        ap.update();
    }
}