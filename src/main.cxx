// Hello you !
//

#include <iostream>
#include <unistd.h>
#include <queue>
#include <vector>
#include "gui/GUI.h"
#include "audio/audio.hpp"

// Macros
#define fori(x) for (int i=0; i<x; i++)

#ifdef DEBUG
#define dprint(x) std::cout << x << std::endl
#else
#define dprint(x)
#endif

#define IAM "Jack Midi Logger"

// Go !
int main (int argc, char * argv[]) {
    char name[256];
    std::queue<std::vector<std::string>> q_midiInputs;

#ifdef DEBUG
    std::snprintf(name, sizeof(name), IAM);
    std::cout << name << " _ " << getpid() << std::endl;
#endif

    fori(argc) {
#ifdef DEBUG
        std::cout << "Arg " << i << ": " << *argv << std::endl;
#endif
        argv++;
    };

    // Create MIDI client
    audio::midi_client client(name, q_midiInputs);
    client.activate();

    if(client.isActivated()) {
        // Create and lauch the GUI main thread
        GUI * Interface = new GUI();
        Interface->root->show();
        Fl::run();
        std::cout << "Bye!" << std::endl;
        return 0;
    } else {
        dprint("MIDI Client creation failed");
        return 1;
    };
}
