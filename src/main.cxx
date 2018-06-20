// Hello you !
//

#include <iostream>
#include <unistd.h>
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

    if(auto&& ClientOpt = audio::midi_client::construct(name)) {
        // Create MIDI client
        audio::midi_client& client = *ClientOpt;


        // Create and lauching the GUI main thread
        GUI * Interface = new GUI();
        Interface->root->show();
        Fl::run();

        client.close();

        std::cout << "Bye!" << std::endl;
        return 0;
    } else {
        dprint("MIDI Client creation failed");
        return 1;
    };
}
