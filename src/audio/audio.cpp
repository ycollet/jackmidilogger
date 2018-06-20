#include "audio.hpp"

std::experimental::optional<audio::midi_client> audio::midi_client::construct(char* name) {
    jack_client_t* client;
    if( (client = jack_client_open(name, JackNoStartServer, NULL)) ) {
        return midi_client(client);
    }

    return std::experimental::nullopt;
};

audio::midi_client::midi_client(jack_client_t* client) {
    me = client;
    setup_callbacks(cb_registration, this);

    jack_activate(me);
}

void audio::midi_client::close() {
    jack_client_close(me);
}

void audio::midi_client::setup_callbacks(JackPortRegistrationCallback c, void * p) {
    jack_set_port_registration_callback(me, c, p);
}

void audio::midi_client::cb_registration(jack_port_id_t port, int regis, void *arg) {
    (void) port;
    (void) regis;
    audio::midi_client * p = (audio::midi_client *) arg;
    const char ** midi_in_clients = jack_get_ports(p->me, NULL, "midi", JackPortIsOutput);

    for(int i=0; midi_in_clients[i]; i++) {
        std::cout << midi_in_clients[i] << std::endl;
    }

    jack_free(midi_in_clients);
};

