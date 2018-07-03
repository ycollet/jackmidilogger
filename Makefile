CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -O2
FLTK_CXXFLAGS = `fltk-config --cflags`
FLTK_LDFLAGS = `fltk-config --ldflags`
JACK_LDFLAGS = -ljack

APP_NAME = jackMidiLogger

all: main

debug: CXXFLAGS += -DDEBUG -g
debug: APP_NAME = debug
debug: main

main: obj/main.o obj/GUI.o obj/audio.o
	$(CXX) $(CXXFLAGS) $(FLTK_CXXFLAGS) $^ -o bin/$(APP_NAME) $(FLTK_LDFLAGS) $(JACK_LDFLAGS)


obj/main.o: src/main.cxx src/gui/GUI.h src/audio/audio.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/GUI.o: src/gui/GUI.cxx src/gui/GUI.h
	$(CXX) $(CXXFLAGS) $(FLTK_CXXFLAGS) -c $< -o $@ $(FLTK_LDFLAGS)

obj/audio.o: src/audio/audio.cpp src/audio/audio.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(JACK_LDFLAGS)
