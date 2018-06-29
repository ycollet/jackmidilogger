// Hello you !
//

#include <iostream>
#include <unistd.h>
#include <queue>
#include <vector>
#include <thread>
#include <algorithm>
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

// Threads
void update_inputs(bool& run, std::queue<std::vector<std::string>>& queue, GUI* gui) {
    while( run ) {
        if( !queue.empty() ) {

            Fl::lock();

            for(int i= 1; i <= gui->sources->nitems(); i++) {
                std::vector<std::string>::iterator isListed = std::find(queue.front().begin(),
                        queue.front().end(), std::string(gui->sources->text(i)) );
                if( isListed != queue.front().end() ) {
                    queue.front().erase(isListed);
                } else {
                    gui->sources->remove(i);
                }
            }

            if( !queue.front().empty() ) {
                for(const std::string& label: queue.front()) {
                    gui->sources->add(label.c_str());
                }
            }
            queue.pop();

            gui->sources->changed();
            gui->sources->redraw();
            Fl::awake();
            Fl::unlock();
        }
    }
}

// Go !
int main (int argc, char * argv[]) {
    char name[256];
    std::queue<std::vector<std::string>> q_midiInputs;
    std::queue<std::vector<std::string>> q_portsStates;
    bool run_update_inputs = true;

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
        GUI * Interface = new GUI(q_portsStates);
        Interface->root->show();

        std::thread thread_update_inputs(update_inputs,std::ref(run_update_inputs), std::ref(q_midiInputs),Interface);

        Fl::lock();
        Fl::run();
        run_update_inputs = false;
        thread_update_inputs.join();

        std::cout << "Bye!" << std::endl;
        return 0;
    } else {
        dprint("MIDI Client creation failed");
        return 1;
    };
}
