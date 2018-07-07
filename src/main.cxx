// Hello you !
//

#include <iostream>
#include <unistd.h>
#include <queue>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
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
        while( !queue.empty() ) {
                Fl::lock();
                // Cycle through items in Check Browser
                int imax = gui->sources->nitems();
                for(int i=1; i <= imax; i++) {
                    if( std::string(gui->sources->text(i)) == queue.front()[1] ) {
                        if( queue.front()[0] == "0" ) {
                            std::cout << "Removed " << gui->sources->text(i) << std::endl;
                            //gui->sources->remove(i);
                        } else if (gui->sources->checked(i) == 1 ) {
                                dprint("gui: checked when reconnected");
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
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    } while(run);
}

void update_ports(bool& run, std::queue<std::vector<std::string>>& queue, audio::midi_client& client) {
    while(run) {
        while(!queue.empty()) {
            client.check_port(queue.front());
            queue.pop();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(125));
    }
}

std::string prettyfy (int type, std::vector<unsigned char> message, audio::midi_notes& midiNotes, audio::midi_control_change& midiCC) {
    std::stringstream res;
    switch(type) {
        case 0x80:
            res << "Note " << midiNotes.getNoteName((int) message[1]);
            res << " off";
            break;
        case 0x90:
            res << "Note " << midiNotes.getNoteName((int) message[1]);
            res << " on, with a velocity of ";
            res << (int) message[2];
            break;
        case 0xb0:
            res << "Control " << midiCC.getControlByNumber((int) message[1]);
            res << " , value of ";
            res << (int) message[2];
        default:
            res << "No prettyfying for this one.";
            break;
    }
    return res.str();
}

void update_display(bool& run, std::queue<std::vector<unsigned char>>& queue, GUI * gui) {
    (void) gui;
    audio::midi_notes midiNotes;
    audio::midi_control_change midiCC;
    while(run) {
        while(!queue.empty()) {
            int type = (int) queue.front()[2] & 0xf0;
            if( ((type == 0x80 || type == 0x90) && gui->notes->value()) ||
                    (type == 0xb0 && gui->cc->value()) ||
                    ( (type > 0xbf || type == 0xa0) && gui->other->value()) ) {

                std::stringstream pretty;
                std::stringstream hexa;

                std::vector<unsigned char>::const_iterator message_first = queue.front().begin()+2;
                std::vector<unsigned char>::const_iterator message_last = queue.front().end();
                std::vector<unsigned char> message(message_first, message_last);

                pretty << "From " << (int) queue.front()[0];
                pretty << " channel " << std::oct << ((queue.front()[2] & 0x0f) + 1);
                pretty << "\n\t";
                hexa << "[" << (int) queue.front()[0] << "]\t";
                for(std::vector<unsigned char>::iterator byte=message.begin(); byte!=message.end(); byte++) {
                    hexa << std::showbase << std::hex << (int) *byte;
                    hexa << " ";
                }
                pretty << prettyfy(type, message, midiNotes, midiCC) << "\n";
                hexa << "\n";
                Fl::lock();
                gui->buffer_pretty->insert(0, pretty.str().c_str());
                gui->buffer_hexa->insert(0, hexa.str().c_str());
                Fl::awake();
                Fl::unlock();
            }
            queue.pop();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
};

bool run_update_inputs = true;
std::thread thread_update_inputs;
bool run_update_display = true;
std::thread thread_update_display;
static void cb_main_window( Fl_Widget * w, void * ) {
    run_update_inputs = false;
    thread_update_inputs.join();
    dprint("main: thread input out");
    run_update_display = false;
    thread_update_display.join();
    dprint("main: thread display out");
    w->hide();
}
// Go !
int main (int argc, char * argv[]) {
    char name[256];
    std::queue<std::vector<std::string>> q_midiInputs;
    std::queue<std::vector<std::string>> q_portsStates;
    std::queue<std::vector<unsigned char>> q_midiMessages;
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
    audio::midi_client client(name, q_midiInputs, q_midiMessages);
    client.activate();

    if(client.isActivated()) {
        // Create and lauch the GUI main thread
        GUI * Interface = new GUI(q_portsStates);

        thread_update_inputs = std::thread(update_inputs,std::ref(run_update_inputs), std::ref(q_midiInputs), std::ref(q_portsStates), Interface);
        thread_update_display = std::thread(update_display,std::ref(run_update_display), std::ref(q_midiMessages), Interface);
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
