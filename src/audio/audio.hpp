#ifndef INCLUDED_H_AUDIO_12062018
#define INCLUDED_H_AUDIO_12062018

#include <iostream>
#include <queue>
#include <vector>
#include <array>
#include <algorithm>
#include <cstring>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <experimental/optional>

namespace audio {
    class midi_notes {
        public:
            std::string getNoteName(int);
        private:
            const std::array<std::string, 12> m_noteNames = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
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
