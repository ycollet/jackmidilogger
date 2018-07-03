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
void update_inputs(bool& run, std::queue<std::vector<std::string>>& queue, std::queue<std::vector<std::string>>& queue_port_removed, GUI* gui) {
    (void) queue_port_removed;
    do {
        bool isListed = false;
        if( !queue.empty() ) {
                Fl::lock();
                // Cycle through items in Check Browser
                int imax = gui->sources->nitems();
                for(int i=1; i <= imax; i++) {
                    if( std::string(gui->sources->text(i)) == queue.front()[1] ) {
                        if( queue.front()[0] == "0" ) {
                            std::cout << "Removed " << gui->sources->text(i) << std::endl;

                            if( gui->sources->checked(i) == 1 ) {
                                dprint("gui: checked when removed");
                                std::vector<std::string> deleted;
                                deleted.push_back(std::to_string(i));
                                deleted.push_back(std::to_string(0));
                                deleted.push_back(gui->sources->text(i));
                                queue_port_removed.push(deleted);
                            }

                            gui->sources->remove(i);
                        } else if (gui->sources->checked(i) == 1 ) {
                                dprint("gui: checked when reconnect");
                                std::vector<std::string> reconnected;
                                reconnected.push_back(std::to_string(i));
                                reconnected.push_back(std::to_string(1));
                                reconnected.push_back(gui->sources->text(i));
                                queue_port_removed.push(reconnected);
                        }
                        isListed = true;
                        break;
                    }
                }
                gui->sources->redraw();
                Fl::unlock();

            if( !isListed ) {
                Fl::lock();
                std::cout << "Found " << queue.front()[1] << std::endl;
                gui->sources->add(queue.front()[1].c_str());
                gui->sources->redraw();
                Fl::unlock();
            }

            Fl::awake();
            queue.pop();
        }
    } while(run);
}

void update_ports(bool& run, std::queue<std::vector<std::string>>& queue, audio::midi_client& client) {
    while(run) {
        while(!queue.empty()) {
            client.check_port(queue.front());
            queue.pop();
        }
    }
}

bool run_update_inputs = true;
std::thread thread_update_inputs;
static void cb_main_window( Fl_Widget * w, void * ) {
    run_update_inputs = false;
    thread_update_inputs.join();
    dprint("main: thread input out");
    w->hide();
}
// Go !


int main (int argc, char * argv[]) {
    char name[256];
    std::queue<std::vector<std::string>> q_midiInputs;
    std::queue<std::vector<std::string>> q_portsStates;
    bool run_update_ports= true;

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

        thread_update_inputs = std::thread(update_inputs,std::ref(run_update_inputs), std::ref(q_midiInputs), std::ref(q_portsStates), Interface);
        std::thread thread_update_ports(update_ports,std::ref(run_update_ports), std::ref(q_portsStates),std::ref(client));

        Interface->root->callback(cb_main_window);
        Interface->root->show();
        Fl::lock();
        Fl::run();
        dprint("main: gui ending");

        run_update_ports= false;
        thread_update_ports.join();
        dprint("main: thread ports out");

        return 0;
    } else {
        dprint("MIDI Client creation failed");
        return 1;
    };
}
