#include "audio.hpp"

std::experimental::optional<audio::midi_client> audio::midi_client::construct(char* name) {
    jack_client_t* client;
    if( (client = jack_client_open(name, JackNoStartServer, NULL)) ) {
        std::cout << "jack: client opened" << std::endl;
        return midi_client(client);
    }

    std::cerr << "jack: client not open" << std::endl;
    return std::experimental::nullopt;
};

audio::midi_client::midi_client(jack_client_t* client) {
    me = client;
    setup_callbacks(cb_registration, this);

    if( jack_activate(me) ) {
        std::cerr << "jack: client not activated" << std::endl;
    }else {
        std::cout << "jack: client activated" << std::endl;
    };
}

audio::midi_client::~midi_client() {
    std::cout << "DESTRUCTOR" << std::endl;
}

void audio::midi_client::close() {
    if( jack_client_close(me) ) {;
        std::cerr << "jack: client not closed" << std::endl;
    }else {
        std::cout << "jack: client closed" << std::endl;
    };
}

void audio::midi_client::setup_callbacks(JackPortRegistrationCallback c, void * p) {
    if( jack_set_port_registration_callback(me, c, p) ) {
        std::cerr << "jack: callback error" << std::endl;
    } else {
        std::cout << "jack: callback attached" << std::endl;
    };

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

