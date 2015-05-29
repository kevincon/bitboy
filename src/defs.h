#pragma once

#define NO_NOISE15
#define NO_PALETTE
#define GNUBOY_NO_MINIZIP
#define GNUBOY_NO_PRINTF
#define GNUBOY_NO_SCREENSHOT
#define NODEBUG
#define GNUBOY_NOSAVELOAD
#define BITBOY_FIXEDRAM 4
#define BITBOY_NOCGB
#define IS_LITTLE_ENDIAN
#define NOTITLE

#ifdef IS_LITTLE_ENDIAN
#define LO 0
#define HI 1
#else
#define LO 1
#define HI 0
#endif

typedef unsigned char byte;

typedef unsigned char un8;
typedef unsigned short un16;
typedef unsigned int un32;

typedef signed char n8;
typedef signed short n16;
typedef signed int n32;

typedef un16 word;
typedef word addr;
