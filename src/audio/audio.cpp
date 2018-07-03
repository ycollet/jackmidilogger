#include "audio.hpp"

audio::midi_client::~midi_client() {
    int unregister = 0;
    for( jack_port_t* p: this->m_ports ) {
        unregister += jack_port_unregister(this->m_client, p);
    }
    if(unregister) {
        std::cerr << "jack: ports not unregistered" << std::endl;
    } else {
        std::cout << "jack: ports unregistered" << std::endl;
    }

    if( jack_deactivate(this->m_client) ) {;
        std::cerr << "jack: client not deactivated" << std::endl;
    }else {
        std::cout << "jack: client deactivated" << std::endl;
    };
    if( jack_client_close(this->m_client) ) {;
        std::cerr << "jack: client not closed" << std::endl;
    }else {
        std::cout << "jack: client closed" << std::endl;
    };
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
        this->init_midi_inputs();
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

void audio::midi_client::init_midi_inputs() {
    const char ** midi_in_clients = jack_get_ports(this->m_client, NULL, "midi", JackPortIsOutput);
    std::vector<std::string> res;

    if(midi_in_clients) {
        std::cout << "jack: init registration" << std::endl;
        for(int i=0; midi_in_clients[i]; i++) {
            res.push_back("1");
            res.push_back(std::string(midi_in_clients[i]));
            this->m_q_midiInputs.push(res);
            res.clear();
        }

        jack_free(midi_in_clients);
        std::cout << "jack: end init registration" << std::endl;
    }
}

void audio::midi_client::cb_registration(jack_port_id_t port, int regis, void *arg) {
    audio::midi_client * p = (audio::midi_client *) arg;
    jack_port_t * reg_port = jack_port_by_id(p->m_client, port);
    std::vector<std::string> res;
    if( !jack_port_is_mine(p->m_client, reg_port) && jack_port_flags(reg_port) == 2 && !std::strcmp(jack_port_type(reg_port), "8 bit raw midi")) {
        res.push_back(std::to_string(regis));
        res.push_back(std::string(jack_port_name(reg_port)));
        p->m_q_midiInputs.push(res);
    }
};

void audio::midi_client::check_port(std::vector<std::string>& ports) {
    std::string name;
    std::string compare;
    bool exists = false;
    name.append("midi_in_");
    name.append(ports[0]);
    compare.append(this->m_name);
    compare.append(":");
    compare.append(name);

    if(!this->m_ports.empty()) {
        for(std::vector<jack_port_t *>::iterator p=this->m_ports.begin();p!=this->m_ports.end(); p++) {
            // if port name seems alreay registered
            if(std::strcmp(jack_port_name(*p), compare.c_str()) == 0) {
                exists = true;
                // if ask activation and port not connected to source asked
                if( (ports[1] == "1") && !(jack_port_connected_to(*p, ports[2].c_str())) ) {
                    jack_port_disconnect(this->m_client, *p);
                    jack_connect(this->m_client, ports[2].c_str(), jack_port_name(*p));
                    std::cout << "jack: reconnect " << ports[2] << " to " << compare << std::endl;
                // if deactivation asked, disconnect and unregister and erase
                } else if ( ports[1] == "0" ) {
                    jack_port_disconnect(this->m_client, *p);
                    jack_port_unregister(this->m_client, *p);
                    this->m_ports.erase(p);
                    std::cout << "jack: unregistered " << compare << std::endl;
                }
                break;
            }
        }
    }
    if(!exists && ports[1] == "1") {
        jack_port_t * p = jack_port_register(this->m_client, name.c_str(), JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
        std::cout << "jack: register " << name << std::endl;
        if(p) {
            this->m_ports.push_back(p);
            jack_connect(this->m_client, ports[2].c_str(), compare.c_str());
        std::cout << "jack: connected " << ports[2] << " to " << name << std::endl;
        } else {
            std::cout << "jack: error creating " << name << std::endl;
        }
    }
}
