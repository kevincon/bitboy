#pragma once

#include "defs.h"

struct pcm
{
	int hz, len;
	int stereo;
	byte *buf;
	int pos;
};

extern struct pcm pcm;
