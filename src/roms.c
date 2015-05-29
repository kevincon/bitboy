#include "roms.h"

#include "mem.h"
#include "hw.h"
#include "rtc.h"
#include "rc.h"

#include <string.h>

static int mbc_table[256] =
{
  0, 1, 1, 1, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 3,
  3, 3, 3, 3, 0, 0, 0, 0, 0, 5, 5, 5, MBC_RUMBLE, MBC_RUMBLE, MBC_RUMBLE, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, MBC_HUC3, MBC_HUC1
};

static int rtc_table[256] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0
};

static int batt_table[256] =
{
  0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
  1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0,
  0
};

static int romsize_table[256] =
{
  2, 4, 8, 16, 32, 64, 128, 256, 512,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 128, 128, 128
  /* 0, 0, 72, 80, 96  -- actual values but bad to use these! */
};

static int ramsize_table[256] =
{
  1, 1, 1, 4, 16,
  4 /* FIXME - what value should this be?! */
};

extern int dmg_pal[4][4];

#ifdef BITBOY_FIXEDRAM
uint8_t ram_mem[8192 * BITBOY_FIXEDRAM];
#endif 

int rom_load(int rom_id)
// almost like original rom load but provides directly data
{
  uint8_t c;
  uint8_t *data, *header;
  // int len = 0, rlen;

  data = (uint8_t *)game_roms[rom_id];

  header = data;
  
  memcpy(rom.name, header+0x0134, 16);
  if (rom.name[14] & 0x80) rom.name[14] = 0;
  if (rom.name[15] & 0x80) rom.name[15] = 0;
  rom.name[16] = 0;

  c = header[0x0147];
  mbc.type = mbc_table[c];
  mbc.batt = batt_table[c];
  rtc.batt = rtc_table[c];

  mbc.romsize = romsize_table[header[0x0148]];
  mbc.ramsize = ramsize_table[header[0x0149]];

  // if (!mbc.romsize) message("unknown ROM size %02X\n", header[0x0148]);
  // if (!mbc.ramsize) message("unknown SRAM size %02X\n", header[0x0149]);

  // XXX extra size ? (ie pad memory when file is smaller than declared size)
  /*
  rlen = 16384 * mbc.romsize;
  rom.bank = realloc(data, rlen);
  if (rlen > len) memset(rom.bank[0]+len, 0xff, rlen - len);
  */
  rom.bank= (byte (*)[16384]) data;

  // static
  #ifdef BITBOY_FIXEDRAM 
  ram.sbank = (void*)&ram_mem;
  #else
  ram.sbank = malloc(8192 * mbc.ramsize);
  #endif 
  // initmem(ram.sbank, 8192 * mbc.ramsize);
  // initmem(ram.ibank, 4096 * 8);

  mbc.rombank = 1;
  mbc.rambank = 0;


  // set custom dmg palette
  // XXX what about 4th palette ?
  for (int i=0;i<3;i++)
    for (int j=0;j<4;j++)
      dmg_pal[i][j] = (int)game_palettes[rom_id][i][j];
  

  // XXX NOPE NO GBC
  c = header[0x0143];
  hw.cgb = 0; // ((c == 0x80) || (c == 0xc0)) ;
  hw.gba = 0; // (hw.cgb && gbamode);

  return 0;
}
