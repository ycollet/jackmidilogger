# Jack MIDI Logger

A JACK MIDI event reader, in C++ using Fltk for the GUI.
The message types can be filtered. You also get two modes of visualisation.

The Pretty one, where the messages are translated as much as possible to human language and musical notation:
![Screenshot](https://github.com/ArnaudE/jackmidilogger/raw/master/doc/sc_pretty.png)

A Raw one, pure hexa porn:
![Screenshot](https://github.com/ArnaudE/jackmidilogger/raw/master/doc/sc_raw.png)

## Before you clone

* I chose to build this in C++ because it has been ten years I didn't write some. I chose to use threads and as much as possible of C++11 sweetness. It might not be the best use of those, but it made me learn a lot, and the result is (in my opinion) fairly usable and sharable, though not polished. But if you see something too big to stay under the public eye, and have the time or energy to point it to me, I'll gladly work on it!
* Channel Pressure is logged with Aftertouch

### To do

* I didn't take the time to find an elegant solution to dynamically unregister and/or update ports when input ports are updated. The application's client ports stay active during the session,
and reconnect if the input ports is coming up again. It keeps the display always right, but it needs to be rethink and rewritten for a more dynamic and precise behavior.
* a2j ports unregister as audio ports, why ?
* Comment
* Clock ticks are not displayed, should they ?
* Display manufacturer when SysEx received
* Take a look at what can be translated with MPE or MIDI-CLI

## Prerequisites
```
C++11
libjack
libfltk
```

## Installing
```
make
make install
```

## Thanks
To everyone working on the C++ language, JACK and Fltk obviously !

