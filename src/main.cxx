// Hello you !
//

#include <iostream>
#include <unistd.h>
#include "gui/GUI.h"

// Macros
#define fori(x) for (int i=0; i<x; i++)

#define IAM "Jack Midi Logger"


// Go !
int main (int argc, char * argv[]) {
    std::cout << IAM << " _ " << getpid() << std::endl;

    fori(argc) {
        std::cout << "Arg " << i << ": " << *argv << std::endl;
        argv++;
    };

    // Create and lauching the GUI main thread
    GUI * Interface = new GUI();
    Interface->root->show();
    Fl::run();

    std::cout << "Bye!" << std::endl;
    return 0;
}
