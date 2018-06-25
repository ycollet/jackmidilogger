#ifndef INCLUDED_H_AUDIO_12062018
#define INCLUDED_H_AUDIO_12062018

#include <iostream>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <experimental/optional>

namespace audio {
    class midi_client {
        public:
            jack_client_t* me;

            ~midi_client();

            void close();

            static std::experimental::optional<midi_client> construct(char*);

        private:
            midi_client(jack_client_t* client);

        void setup_callbacks(JackPortRegistrationCallback, void *);
        static void cb_registration(jack_port_id_t port, int regis, void *arg);
    };
}


#endif
