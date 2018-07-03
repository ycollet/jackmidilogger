#ifndef INCLUDED_H_AUDIO_12062018
#define INCLUDED_H_AUDIO_12062018

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <cstring>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <experimental/optional>

namespace audio {
    class midi_client {
        public:
            midi_client(char* name, std::queue<std::vector<std::string>>& queue) : m_name(name), m_q_midiInputs(queue) {};
            ~midi_client();

            void activate();
            void setup_callbacks(JackPortRegistrationCallback, void *);
            bool isActivated();
            void check_port(std::vector<std::string>&);
        private:
            jack_client_t* m_client;
            const char* m_name;
            std::queue<std::vector<std::string>>& m_q_midiInputs;
            unsigned int m_jack_errors = 0;
            static void cb_registration(jack_port_id_t port, int regis, void *arg);
            void init_midi_inputs();
            std::vector<jack_port_t *> m_ports;
    };
}


#endif
