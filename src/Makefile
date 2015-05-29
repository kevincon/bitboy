# Standard Makefile for bitbox
NAME= bitboy
GAME_C_FILES= bitbox_lcd.c bitbox_emu.c bitbox_sys.c\
	refresh.c lcdc.c cpu.c mem.c hw.c sound.c rtc.c\
	roms.c

# gnuboy options + added bitboy options to simplify further
GAME_C_OPTS += -DNO_NOISE15 -DNO_PALETTE -DGNUBOY_NO_MINIZIP -DGNUBOY_NO_PRINTF
GAME_C_OPTS += -DGNUBOY_NO_SCREENSHOT -DNODEBUG -DGNUBOY_NOSAVELOAD -DBITBOY_FIXEDRAM=4
GAME_C_OPTS += -DBITBOY_NOCGB -DIS_LITTLE_ENDIAN

# this is done because many elements are not used
GAME_C_OPTS += -Wno-unused-variable -Wno-unused-function
GAME_C_OPTS += -DNOTITLE

.PHONY: all
.DEFAULT: all
all: roms.auto.h

# Get the bitbox repo (https://github.com/makapuf/bitbox) and export its location as the
# BITBOX environment variable so we can include its Makefile
include $(BITBOX)/lib/bitbox.mk

# package roms as a header file, parsing name, size & other characteristics
roms.auto.h: ../roms
	python ../scripts/mk_rom.py ../roms/* > roms.auto.h

clean::
	rm -f roms.auto.h
