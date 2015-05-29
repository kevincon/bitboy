/*
 * bitbox interfaces (c) makapuf@gmail.com
 */

// main (as game_frame) is in bitbox_emu.c

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h> // for memcpy

#include "gnuboy.h"
#include "fb.h"
#include "hw.h"

#include <bitbox.h>


#define BITBOX_VIDBUF_LEN (160*144)

extern uint16_t audio_write[BITBOX_SNDBUF_LEN];
uint8_t audio_buffer[BITBOX_SNDBUF_LEN];

// put to ccmram to save space on sram
uint8_t video_buffer1[BITBOX_VIDBUF_LEN] __attribute__ ((section (".ccm")));
uint8_t video_buffer2[BITBOX_VIDBUF_LEN] __attribute__ ((section (".ccm")));

static uint8_t *lcd_display_buffer;
uint8_t framebuffer_flip; // ask to flip framebuf

struct fb fb; // here the frame buffer contains emulated pixels, the translation to real world is done in lines

void vid_init()
// force to 160x144 8bits
// will be translated back to 640x480 16 bits in game_line
{
	fb.w = 160;
	fb.h = 144;
	fb.pitch = 160;
	// not used
	fb.pelsize = 1;
	fb.indexed = 0;
	
	// set for the line rendering 0BGR, 12 bits
	// R
	fb.cc[0].r = 3; // 8 bits->5
	fb.cc[0].l = 10;
	// G
	fb.cc[1].r = 3;
	fb.cc[1].l = 5;
	// B
	fb.cc[2].r = 3;
	fb.cc[2].l = 0;


	fb.ptr = (byte *)&video_buffer1[0]; // = draw buffer
	lcd_display_buffer = &video_buffer2[0];

	fb.dirty = 1;
	fb.enabled = 1;
}

/* hw.pad values 
#define PAD_RIGHT  0x01
#define PAD_LEFT   0x02
#define PAD_UP     0x04
#define PAD_DOWN   0x08
#define PAD_A      0x10
#define PAD_B      0x20
#define PAD_SELECT 0x40
#define PAD_START  0x80
*/

// mapping of bitbox to gnuboy hw.pad bits

const int joystick_pos[8] = {
	gamepad_right, 
	gamepad_left, 
	gamepad_up,
	gamepad_down, 
	gamepad_A,
	gamepad_B,
	gamepad_select,
	gamepad_start
};

void gamepad_poll()
// direct mapping of bitbox events to hardware hw.pad, bypassing all of events ev_* functions.
{
	static uint16_t prev_state; // to detect changes since last time.
	hw.pad=0;
	for (int i=0;i<8;i++) {
		if (gamepad_buttons[0] & joystick_pos[i]) 
				hw.pad |= 1<<i;
	}
}


void vid_preinit()
{
}

void vid_close()
{
}

void vid_settitle(char *title)
{
	//SDL_WM_SetCaption(title, title);
}


void vid_begin() // as in begin FRAME. We're synced to vsync here
{
	uint8_t *tmp;

	// flip draw buffer & display
	tmp=lcd_display_buffer; 
	lcd_display_buffer=fb.ptr;
	fb.ptr=tmp; // current drawing framebuffer
}

void vid_end() // as in end frame
// 
{
	// XXX flip framebuffer (wait vsync ?)
}


#include "pcm.h"

struct pcm pcm;

static volatile int audio_done;

void game_snd_buffer(uint16_t *buffer, int len) 
// this callback is called each time we need to fill the buffer
// should be done within one line (??)
{
	if (pcm.buf) {
		uint16_t *b=buffer;
		for (int i=0;i<len;i++)
			*b++=pcm.buf[i]*0x101;
	}
	audio_done = 1;
}

void pcm_init()
{
	pcm.hz = BITBOX_SAMPLERATE;
	pcm.stereo = 0; // mono sound 
	pcm.len = BITBOX_SNDBUF_LEN; // uint8_t samples per 60Hz frame 
	pcm.buf = audio_buffer; // dont make a new one
	pcm.pos = 0;
	memset(pcm.buf, 0, pcm.len);
	// now start bitbox audio
	// audio_init(); // already initialized ?
	// audio_on = 1;
}

int pcm_submit() // time sync also done here from the buffer which MUST be one frame long.
{
	if (pcm.pos < pcm.len) return 1;
    #ifdef EMULATED
    while (!audio_done) // attend pile 
		SDL_Delay(2);
	#endif 

	audio_done = 0;
	pcm.pos = 0;
	return 1;
}

void pcm_close()
{
}



void gb_die(char *fmt, ...)
{

#ifdef EMULATED
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(1);
#endif
}
