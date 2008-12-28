#include "mmc3.h"
#include "mappers/mapper.h"
#include "nes/nes.h"
#include "nes/ppu/ppu.h"

static void (*sync)();
static u8 command;
static u8 prg[2],chr[8];
static u8 mirror;
static u8 sramenabled;
static u8 irqlatch,irqcounter,irqenabled,irqreload;

void mmc3_sync()
{
	mmc3_syncprg(0xFF,0);
	if(nes->rom->chrsize)
		mmc3_syncchr(0xFF,0);
	else
		mmc3_syncvram(7,0);
	if(nes->rom->mirroring & 8)
	   ppu_setmirroring(MIRROR_4);
	else
		mmc3_syncmirror();
	mmc3_syncsram();
}

u8 mmc3_getprgbank(int n)
{
	u8 b[4];

	b[0] = prg[0];
	b[1] = prg[1];
	b[2] = 0x3E;
	b[3] = 0x3F;
	if(n & 1)
		return(b[n]);
	return(b[n ^ ((command & 0x40) >> 5)]);
}

u8 mmc3_getchrbank(int n)
{
	return(chr[n ^ ((command & 0x80) >> 5)]);
}

void mmc3_syncprg(int a,int o)
{
	mem_setprg8(0x8,(mmc3_getprgbank(0) & a) | o);
	mem_setprg8(0xA,(mmc3_getprgbank(1) & a) | o);
	mem_setprg8(0xC,(mmc3_getprgbank(2) & a) | o);
	mem_setprg8(0xE,(mmc3_getprgbank(3) & a) | o);
}

void mmc3_syncchr(int a,int o)
{
	int i;

	for(i=0;i<8;i++)
		mem_setchr1(i,(mmc3_getchrbank(i) & a) | o);
}

void mmc3_syncvram(int a,int o)
{
	int i;

	for(i=0;i<8;i++)
		mem_setvram1(i,(mmc3_getchrbank(i) & a) | o);
}

void mmc3_syncsram()
{
	if(sramenabled)
		mem_setsram8(6,0);
	else
		mem_unsetcpu8(6);
}

void mmc3_syncmirror()
{
	if(mirror & 1)
	   ppu_setmirroring(MIRROR_H);
	else
	   ppu_setmirroring(MIRROR_V);
}

void mmc3_init(void (*s)())
{
	int i;

	nes_setsramsize(2);
	for(i=8;i<0x10;i++)
		mem_setwrite(i,mmc3_write);
	sync = s;
	command = 0;
	prg[0] = 0x3C;
	prg[1] = 0x3D;
	chr[0] = 0;
	chr[1] = 1;
	chr[2] = 2;
	chr[3] = 3;
	chr[4] = 4;
	chr[5] = 5;
	chr[6] = 6;
	chr[7] = 7;
	mirror = 0;
	sramenabled = 1;
	irqcounter = irqlatch = 0;
	irqenabled = 0;
	sync();
}

void mmc3_write(u32 addr,u8 data)
{
	switch(addr & 0xE001) {
		case 0x8000:
			command = data;
			sync();
			break;
		case 0x8001:
			switch(command & 7) {
				case 0:
					data &= 0xFE;
					chr[0] = data | 0;
					chr[1] = data | 1;
					break;
				case 1:
					data &= 0xFE;
					chr[2] = data | 0;
					chr[3] = data | 1;
					break;
				case 2:
					chr[4] = data;
					break;
				case 3:
					chr[5] = data;
					break;
				case 4:
					chr[6] = data;
					break;
				case 5:
					chr[7] = data;
					break;
				case 6:
					prg[0] = data & 0x3F;
					break;
				case 7:
					prg[1] = data & 0x3F;
					break;
			}
			sync();
			break;
		case 0xA000:
			mirror = data;
			sync();
			break;
		case 0xA001:
		//keep enabled all the time for now
//			sramenabled = data & 0x80;
			sync();
			break;
		case 0xC000:
			irqlatch = data;
			break;
		case 0xC001:
			irqcounter = 0;
			irqreload = 1;
			break;
		case 0xE000:
			irqenabled = 0;
			break;
		case 0xE001:
			irqenabled = 1;
			break;
	}
}

void mmc3_line(int line,int pcycles)
{
	u8 n;

	//counter is only clocked on visible lines when ppu is rendering
	if((line < 20 || line > 260) && (line != 1000))
		return;

	if((nes->ppu.ctrl1 & 0x18) == 0 || (nes->ppu.ctrl1 & 0x18) != 0x18)
		return;

	if(irqenabled == 0)
		return;

	if(line == 20) {
		irqcounter = irqreload;
		return;
	}

	n = irqcounter;
	if((irqcounter == 0) || irqreload) {
		irqreload = 0;
		irqcounter = irqlatch;
	}
	else
		irqcounter--;

	if(n && (irqcounter == 0) && irqenabled)
		dead6502_irq();
}

void mmc3_state(int mode,u8 *data)
{
	STATE_ARRAY_U8(prg,2);
	STATE_ARRAY_U8(chr,8);
	STATE_U8(command);
	STATE_U8(mirror);
	STATE_U8(sramenabled);
	STATE_U8(irqlatch);
	STATE_U8(irqcounter);
	STATE_U8(irqenabled);
	sync();
}
