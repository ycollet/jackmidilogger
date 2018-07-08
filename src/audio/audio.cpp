#include "audio.hpp"

std::string audio::midi_notes::getNoteName(int noteNumber) {
    if( noteNumber > 127 || noteNumber < 0 ) {
        return std::string("Undefined");
    } else {
        std::string res;
        res += m_noteNames[noteNumber%12];
        res += std::to_string(std::div(noteNumber, 12).quot - 1);
        return res;
    }
}

std::string audio::midi_control_change::getControlByNumber(int ccNumber) {
    if( ccNumber > 127 || ccNumber < 0 ) {
        return std::string("Undefined");
    } else {
        return this->m_controlChange[ccNumber];
    }
}

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
        if( jack_set_port_registration_callback(this->m_client, cb_registration, this) || jack_set_process_callback(this->m_client, cb_process, this) ) {
            this->m_jack_errors++;
            std::cerr << "jack: callback error" << std::endl;
        } else {
            std::cout << "jack: callback attached" << std::endl;
        };
    } else {
        this->m_jack_errors++;
        std::cerr << "jack: client not open" << std::endl;
    }

    if( jack_activate(this->m_client) ) {
        this->m_jack_errors++;
        std::cerr << "jack: client not activated" << std::endl;
    }else {
        std::cout << "jack: client activated" << std::endl;
        this->init_midi_inputs();
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
    if( !jack_port_is_mine(p->m_client, reg_port) && jack_port_flags(reg_port)%2 == 0) {
        if( !regis ) {
            res.push_back(std::to_string(regis));
            res.push_back(std::string(jack_port_name(reg_port)));
            p->m_q_midiInputs.push(res);
        } else if ( !std::strcmp(jack_port_type(reg_port), "8 bit raw midi")) {
            res.push_back(std::to_string(regis));
            res.push_back(std::string(jack_port_name(reg_port)));
            p->m_q_midiInputs.push(res);
        }
    }
};

int audio::midi_client::cb_process(jack_nframes_t nframes, void* arg) {
    audio::midi_client * c = (audio::midi_client *) arg;
    for(std::vector<jack_port_t *>::iterator port=c->m_ports.begin(); port!=c->m_ports.end(); port++) {
        void * port_buff = jack_port_get_buffer(*port, nframes);
        jack_midi_event_t in_event;
        jack_nframes_t event_count = jack_midi_get_event_count(port_buff);

        if(event_count) {
            char * aliases[2];
            std::vector<unsigned char> message;
            jack_port_get_aliases(*port, aliases);
            for(unsigned int i=0; i < event_count; i++) {
                jack_midi_event_get(&in_event, port_buff, i);
                message.push_back(std::atoi(aliases[0]));
                message.push_back(in_event.size);
                for(long unsigned int j=0; j<in_event.size; j++) {
                    message.push_back(*(in_event.buffer+j));
                }
                c->m_q_midiMessages.push(message);
                message.clear();
            }
        }
    }

    return 0;
}

void audio::midi_client::check_port(std::vector<std::string>& ports) {
    std::string name;
    std::string compare;
    bool exists = false;
    name.append("midi_in_");
    if( ports[0].size() == 1 ) {
        ports[0].insert(0, std::to_string(0));
    }
    name.append(ports[0]);
    compare.append(this->m_name);
    compare.append(":");
    compare.append(name);

    if(!this->m_ports.empty()) {
        for(std::vector<jack_port_t *>::iterator p=this->m_ports.begin();p!=this->m_ports.end(); p++) {
            if( jack_port_connected_to(*p, ports[2].c_str()) || !std::strcmp(jack_port_name(*p), compare.c_str())) {
                exists = true;
                if( (ports[1] == "1") ) {
                    jack_port_disconnect(this->m_client, *p);
                    //jack_port_set_name(*p, name.c_str());
                    //jack_port_set_alias(*p, ports[0].c_str());
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
        if(p) {
            if ( !jack_connect(this->m_client, ports[2].c_str(), compare.c_str()) ) {
                std::cout << "jack: connected " << ports[2] << " to " << name << std::endl;
                jack_port_set_alias(p, ports[0].c_str());
                this->m_ports.push_back(p);
            } else {
                std::cout << "jack: source " << name;
                std::cout << " does not exists, reconnect" << std::endl;
                jack_port_unregister(this->m_client, p);
            };
        } else {
            std::cout << "jack: error creating " << name << std::endl;
        }
    }
}
