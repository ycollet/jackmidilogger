#ifndef INCLUDED_H_AUDIO_12062018
#define INCLUDED_H_AUDIO_12062018

#include <iostream>
#include <queue>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <cstring>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <experimental/optional>

namespace audio {
    class midi_control_change {
        public:
            std::string getControlByNumber(int);
        private:
            std::array<std::string, 128> m_controlChange = {
                "Bank Select",
                "Modulation Wheel",
                "Breath Contoller",
                "Undefined",
                "Foot Controller",
                "Portamento Time",
                "Data Entry MSB",
                "Main Volume",
                "Balance",
                "Undefined",
                "Pan",
                "0Ch",
                "Effect Control 1",
                "Effect Control 2",
                "Undefined",
                "Undefined",
                "General Purpose Controllers 1",
                "General Purpose Controllers 2",
                "General Purpose Controllers 3",
                "General Purpose Controllers 4",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "LSB for Controllers 0",
                "LSB for Controllers 1",
                "LSB for Controllers 2",
                "LSB for Controllers 3",
                "LSB for Controllers 4",
                "LSB for Controllers 5",
                "LSB for Controllers 6",
                "LSB for Controllers 7",
                "LSB for Controllers 8",
                "LSB for Controllers 9",
                "LSB for Controllers 10",
                "LSB for Controllers 11",
                "LSB for Controllers 12",
                "LSB for Controllers 13",
                "LSB for Controllers 14",
                "LSB for Controllers 15",
                "LSB for Controllers 16",
                "LSB for Controllers 17",
                "LSB for Controllers 18",
                "LSB for Controllers 19",
                "LSB for Controllers 20",
                "LSB for Controllers 21",
                "LSB for Controllers 22",
                "LSB for Controllers 23",
                "LSB for Controllers 24",
                "LSB for Controllers 25",
                "LSB for Controllers 26",
                "LSB for Controllers 27",
                "LSB for Controllers 28",
                "LSB for Controllers 29",
                "LSB for Controllers 30",
                "LSB for Controllers 31",
                "Damper Pedal (Sustain)",
                "Portamento",
                "Sostenuto",
                "Soft Pedal",
                "Legato Footswitch",
                "Hold 2",
                "SC1 - Sound Variation",
                "SC2 - Timbre/Harmonic Content",
                "SC3 - Release Time",
                "SC4 - Attack Time",
                "SC5 – Brightness",
                "SC6",
                "SC7",
                "SC8",
                "SC9",
                "SC10",
                "General Purpose Controllers 5",
                "General Purpose Controllers 6",
                "General Purpose Controllers 7",
                "General Purpose Controllers 8",
                "Portamento Control",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Effects 1 Depth",
                "Effects 2 Depth",
                "Effects 3 Depth ",
                "Effects 4 Depth",
                "Effects 5 Depth",
                "Data Increment",
                "Data Decrement",
                "Non-Registered Parameter Number LSB",
                "Non-Registered Parameter Number LSB",
                "Registered Parameter Number LSB",
                "Registered Parameter Number MSB",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Undefined",
                "Reset All Controllers",
                "Local Control",
                "All Notes Off",
                "Omni Off",
                "Omni On",
                "Mono On (Poly Off)",
                "Poly On (Mono Off)"
            };
    };

    class midi_notes {
        public:
            std::string getNoteName(int);
        private:
            const std::array<std::string, 12> m_noteNames = {
                "C",
                "C#",
                "D",
                "D#",
                "E",
                "F",
                "F#",
                "G",
                "G#",
                "A",
                "A#",
                "B"
            };
    };

    class midi_client {
        public:
            midi_client(char* name, std::queue<std::vector<std::string>>& queue_inputs, std::queue<std::vector<unsigned char>>& queue_messages) : m_name(name), m_q_midiInputs(queue_inputs), m_q_midiMessages(queue_messages) {};
            ~midi_client();
            void activate();
            bool isActivated();
            void check_port(std::vector<std::string>&);
        private:
            jack_client_t* m_client;
            const char* m_name;
            std::queue<std::vector<std::string>>& m_q_midiInputs;
            std::queue<std::vector<unsigned char>>& m_q_midiMessages;
            unsigned int m_jack_errors = 0;
            static void cb_registration(jack_port_id_t port, int regis, void *arg);
            static int cb_process(jack_nframes_t nframes, void* arg);
            void init_midi_inputs();
            std::vector<jack_port_t *> m_ports;
    };
}


#endif
