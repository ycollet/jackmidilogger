.DEFAULT_GOAL := main

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -O2 -std=c++11
FLTK_CXXFLAGS = `fltk-config --cflags`
FLTK_LDFLAGS = `fltk-config --ldflags`
JACK_CXXFLAGS = -Wno-deprecated-declarations
JACK_LDFLAGS = -ljack

APP_NAME = jackmidilogger
DESKTOP_FILE = jackmidilogger.desktop
PREFIX = /usr/local

debug: CXXFLAGS += -DDEBUG -g
debug: APP_NAME = debug
debug: main

install: bin/$(APP_NAME)
	cp -v $^ $(PREFIX)/bin/
	cp -v $(DESKTOP_FILE) $(PREFIX)/share/applications/

uninstall:
	rm -v $(PREFIX)/bin/$(APP_NAME)
	rm -v $(PREFIX)/applications/$(DESKTOP_FILE)

main: src/main.cxx obj/GUI.o obj/audio.o
	$(CXX) $(CXXFLAGS) $(FLTK_CXXFLAGS) $^ -o bin/$(APP_NAME) $(FLTK_LDFLAGS) $(JACK_LDFLAGS)

obj/GUI.o: src/gui/GUI.cxx src/gui/GUI.h
	$(CXX) $(CXXFLAGS) $(FLTK_CXXFLAGS) -c $< -o $@ $(FLTK_LDFLAGS)

obj/audio.o: src/audio/audio.cpp src/audio/audio.hpp
	$(CXX) $(CXXFLAGS) $(JACK_CXXFLAGS) -c $< -o $@ $(JACK_LDFLAGS)
