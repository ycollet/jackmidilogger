#ifndef INCLUDED_H_AUDIO_12062018
#define INCLUDED_H_AUDIO_12062018

#include <iostream>
#include <queue>
#include <vector>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <experimental/optional>

namespace audio {
    class midi_client {
        public:
            midi_client(char* name, std::queue<std::vector<std::string>>& queue) : m_name(name), m_q_midiInputs(queue) {};
            ~midi_client();

            void activate();
            void close();
            void setup_callbacks(JackPortRegistrationCallback, void *);
            bool isActivated();
            static void cb_registration(jack_port_id_t port, int regis, void *arg);
        private:
            jack_client_t* m_client;
            const char* m_name;
            std::queue<std::vector<std::string>>& m_q_midiInputs;
            unsigned int m_jack_errors = 0;
    };
}


#endif
