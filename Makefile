.DEFAULT_GOAL := main

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -O2 -std=c++11
FLTK_CXXFLAGS = `fltk-config --cflags`
FLTK_LDFLAGS = `fltk-config --ldflags`
JACK_CXXFLAGS = -Wno-deprecated-declarations
JACK_LDFLAGS = -ljack

APP_NAME = jackMidiLogger

debug: CXXFLAGS += -DDEBUG -g
debug: APP_NAME = debug
debug: main

main: src/main.cxx obj/GUI.o obj/audio.o
	$(CXX) $(CXXFLAGS) $(FLTK_CXXFLAGS) $^ -o bin/$(APP_NAME) $(FLTK_LDFLAGS) $(JACK_LDFLAGS)

obj/GUI.o: src/gui/GUI.cxx src/gui/GUI.h
	$(CXX) $(CXXFLAGS) $(FLTK_CXXFLAGS) -c $< -o $@ $(FLTK_LDFLAGS)

obj/audio.o: src/audio/audio.cpp src/audio/audio.hpp
	$(CXX) $(CXXFLAGS) $(JACK_CXXFLAGS) -c $< -o $@ $(JACK_LDFLAGS)
