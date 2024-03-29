// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "GUI.h"

GUI::GUI(std::queue<std::vector<std::string>>& res_q_portStates, char* name) : q_portsStates(res_q_portStates) {
  { root = new Fl_Double_Window(945, 680, name);
    root->user_data((void*)(this));
    { sources = new Fl_Check_Browser(15, 125, 300, 550, "Sources");
      sources->labelfont(13);
      sources->textfont(13);
      sources->align(Fl_Align(FL_ALIGN_TOP));
      sources->when(FL_WHEN_CHANGED);
      sources->callback((Fl_Callback *)check_sources, this);
    } // Fl_Check_Browser* sources
    { buffer_hexa = new Fl_Text_Buffer();
      buffer_hexa->insert(0, "I'm Ugly");
    } // Fl_Text_Buffer* buffer_hexa
    { buffer_pretty = new Fl_Text_Buffer();
      buffer_pretty->insert(0, "I'm Pretty");
    } // Fl_Text_Buffer* buffer_pretty
    { messages = new Fl_Text_Display(330, 125, 600, 550, "Messages");
      messages->box(FL_DOWN_BOX);
      messages->color(FL_GRAY0);
      messages->textfont(13);
      messages->textcolor(FL_BACKGROUND2_COLOR);
      // Display Prettyfied data by default
      messages->buffer(buffer_pretty);
    } // Fl_Text_Display* messages
    { options = new Fl_Pack(330, 15, 600, 80);
      options->type(1);
      { notes = new Fl_Light_Button(570, 35, 80, 40, "Notes");
        notes->selection_color((Fl_Color)2);
        // MIDI Notes displayed by default
        notes->value(1);
      } // Fl_Light_Button* notes
      { cc = new Fl_Light_Button(660, 35, 80, 40, "CC");
        cc->selection_color((Fl_Color)2);
        cc->value(1);
      } // Fl_Light_Button* cc
      { aftertouch = new Fl_Light_Button(750, 35, 80, 40, "AfterTch");
        aftertouch->selection_color((Fl_Color) 2);
      }
      { sysex = new Fl_Light_Button(840, 35, 80, 40, "SysEx");
        sysex->selection_color((Fl_Color) 2);
        sysex->value(1);
      }
      { song = new Fl_Light_Button(930, 35, 80, 40, "Song");
        song->selection_color((Fl_Color) 2);
      }
      { other = new Fl_Light_Button(1020, 35, 80, 40, "Other");
        other->selection_color((Fl_Color)2);
      } // Fl_Light_Button* other
      { pretty = new Fl_Light_Button(1920, 35, 80, 40, "Pretty");
        pretty->selection_color((Fl_Color)1);
        pretty->value(1);
        pretty->when(FL_WHEN_CHANGED);
        pretty->callback( (Fl_Callback*) butPushPretty, this);
      } // Fl_Light_Button* pretty
      options->end();
    } // Fl_Pack* options
    root->end();
  } // Fl_Double_Window* root
}

void GUI::butPushPretty(Fl_Light_Button* b, GUI * t) {
    ( b->value() ) ?
        t->messages->buffer(t->buffer_pretty)
        :
        t->messages->buffer(t->buffer_hexa);
}

void GUI::check_sources(Fl_Widget * b, GUI * t) {
    (void) b;
    std::vector<std::string> port_state;
    int index = t->sources->value();

    port_state.push_back(std::to_string(index));
    port_state.push_back(std::to_string(t->sources->checked(index)));
    port_state.push_back(t->sources->text(index));
    t->q_portsStates.push(port_state);
}

