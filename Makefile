CXX = g++
CXXFLAGS = -Wall -Wextra -Werror
FLTK_CXXFLAGS = `fltk-config --cflags`
FLTK_LDFLAGS = `fltk-config --ldflags`
JACK_LDFLAGS = -ljack

APP_NAME = "jackMidiLogger"

main: obj/main.o obj/GUI.o
	$(CXX) $(CXXFLAGS) $(FLTK_CXXFLAGS) $^ -o bin/$(APP_NAME) $(FLTK_LDFLAGS)

debug: obj/main.o obj/GUI.o
	$(CXX) $(CXXFLAGS) -g $(FLTK_CXXFLAGS) $^ -o bin/$(APP_NAME)_debug $(FLTK_LDFLAGS)

obj/main.o: src/main.cxx src/gui/GUI.h
	$(CXX) $(CXXFLAGS) $(FLTK_CXXFLAGS) -c $< -o $@ $(FLTK_LDFLAGS)

obj/GUI.o: src/gui/GUI.cxx src/gui/GUI.h
	$(CXX) $(CXXFLAGS) $(FLTK_CXXFLAGS) -c $< -o $@ $(FLTK_LDFLAGS)
