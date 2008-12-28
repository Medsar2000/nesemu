#ifndef __mmc3_h__
#define __mmc3_h__

#include "defines.h"

#define MMC3A	0
#define MMC3B	1
#define MMC3C	2

void mmc3_sync();
u8 mmc3_getprgbank(int n);
u8 mmc3_getchrbank(int n);
void mmc3_syncprg(int a,int o);
void mmc3_syncchr(int a,int o);
void mmc3_syncvram(int a,int o);
void mmc3_syncsram();
void mmc3_syncmirror();
void mmc3_init(void (*s)());
void mmc3_write(u32 addr,u8 data);
void mmc3_line(int line,int pcycles);
void mmc3_state(int mode,u8 *data);

#endif
