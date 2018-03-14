Requirements:
- [Arduino.mk](https://github.com/sudar/Arduino-Makefile) (distro packages seem to work)
- Arduino core SDK 1.6.10+

To compile for Arduino, using neopixel LEDs, just run `make`.
You may also need `-I /usr/share/arduino/` pointing to the directory with Arduino.mk.
Use `make upload` to install on the Arduino.

To compile for Arduboy (simulates LEDs on screen):
    make TARGET=arduboy

The environment variable `ARDUINO_DIR` can be set to the path to the Arduino SDK,
if it's not detected automatically.
