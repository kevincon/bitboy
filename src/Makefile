# Standard Makefile for bitbox
NAME= bitboy
GAME_C_FILES= bitbox_lcd.c bitbox_emu.c bitbox_sys.c\
	refresh.c lcdc.c cpu.c mem.c hw.c sound.c rtc.c\
	roms.c
 
# gnuboy options + added bitboy options to simplify further
GAME_C_OPTS += -DNO_NOISE15 -DNO_PALETTE -DGNUBOY_NO_MINIZIP -DGNUBOY_NO_PRINTF -DGNUBOY_NO_SCREENSHOT -DNODEBUG -DGNUBOY_NOSAVELOAD
GAME_C_OPTS += -DBITBOY_FIXEDRAM=4 -DBITBOY_NOCGB -DIS_LITTLE_ENDIAN  

# this is done because many elements are not used
GAME_C_OPTS += -Wno-unused-variable -Wno-unused-function
GAME_C_OPTS += -DNOTITLE

# standard makefile
include $(BITBOX)/lib/bitbox.mk



# package roms in c file, parsing name, size & other characteristics to populate menu
roms.c: ../roms
	python ../scripts/mk_rom.py ../roms/* > roms.c 

clean:: 
	rm -f roms.c
