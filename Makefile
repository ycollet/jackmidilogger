CXX = g++
CXXFLAGS = -Wall -Wextra -Werror
LDFLAGS = -ljack `fltk-config --ldflags`

APP_NAME = "jackMidiLogger"

main: src/main.cxx
	$(CXX) $(CXXFLAGS) -o $(APP_NAME) $^ $(LDFLAGS)

debug: src/main.cxx
	$(CXX) $(CXXFLAGS) -g -o $(APP_NAME) $^ $(LDFLAGS)
