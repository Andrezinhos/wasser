#pragma once
#include <iostream>
#include <cstdlib>
#include <string>

class States{
public:
    std::string name;
    bool running;
    bool multiChannel = false;
    bool debugMode = false;
    int choice;
    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else   
        system("clear");
#endif
    }
	void colorScreen() {
#ifdef _WIN32
        system("powershell -Command \"$Host.UI.RawUI.BackgroundColor = 'DarkBlue'; Clear-Host\"");
#else   
        system("clear");
#endif
    }

};
extern States state;
