# Standard Makefile for bitbox
NAME= bitboy
GAME_C_FILES= bitbox_lcd.c bitbox_emu.c bitbox_sys.c\
	refresh.c lcdc.c cpu.c mem.c hw.c sound.c rtc.c\
	roms.c

# this is done because many elements are not used
GAME_C_OPTS += -Wno-unused-variable -Wno-unused-function

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
