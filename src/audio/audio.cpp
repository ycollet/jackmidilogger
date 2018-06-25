#include "audio.hpp"

audio::midi_client::~midi_client() {
    this->close();
}

void audio::midi_client::activate() {
    if( (this->m_client = jack_client_open(this->m_name, JackNoStartServer, NULL)) ) {
        std::cout << "jack: client opened" << std::endl;
    } else {
        this->m_jack_errors++;
        std::cerr << "jack: client not open" << std::endl;
    }

    setup_callbacks(cb_registration, this);

    if( jack_activate(this->m_client) ) {
        this->m_jack_errors++;
        std::cerr << "jack: client not activated" << std::endl;
    }else {
        std::cout << "jack: client activated" << std::endl;
    };
}

void audio::midi_client::close() {
    if( jack_client_close(this->m_client) ) {;
        std::cerr << "jack: client not closed" << std::endl;
    }else {
        std::cout << "jack: client closed" << std::endl;
    };
}

void audio::midi_client::setup_callbacks(JackPortRegistrationCallback c, void * p) {
    if( jack_set_port_registration_callback(this->m_client, c, p) ) {
        this->m_jack_errors++;
        std::cerr << "jack: callback error" << std::endl;
    } else {
        std::cout << "jack: callback attached" << std::endl;
    };

}

bool audio::midi_client::isActivated() {
    return ( this->m_jack_errors > 0 ) ? false : true;
}

void audio::midi_client::cb_registration(jack_port_id_t port, int regis, void *arg) {
    (void) port;
    (void) regis;
    audio::midi_client * p = (audio::midi_client *) arg;
    const char ** midi_in_clients = jack_get_ports(p->m_client, NULL, "midi", JackPortIsOutput);
    std::vector<std::string> midiInputArray;

    std::cout << std::char_traits<const char*>::length(midi_in_clients) << std::endl;
    for(int i=0; midi_in_clients[i]; i++) {
        midiInputArray.push_back(midi_in_clients[i]);
    }

    jack_free(midi_in_clients);

    p->m_q_midiInputs.push(midiInputArray);
};

