# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

USER_LIB_PATH  = libraries

ifeq (x$(TARGET), xarduboy)
    BOARD_TAG = leonardo
    ARDUINO_LIBS = Arduboy2 EEPROM
    LOCAL_INO_SRCS = RackLedsArduboy.ino
    CPPFLAGS     = -DRL_TARGET_ARDUINO=1
else
    BOARD_TAG    = uno
    ARDUINO_LIBS = FastLED
    LOCAL_INO_SRCS = RackLeds.ino
    CPPFLAGS     = -DRL_TARGET_FASTLED=1
endif

include Arduino.mk

