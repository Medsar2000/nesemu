/***************************************************************************
 *   Copyright (C) 2006-2009 by Dead_Body   *
 *   jamesholodnak@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __input_h__
#define __input_h__

#include "nes/nes.h"

#define INPUT_SCREENSHOT		0x00400000
#define INPUT_DISKFLIP			0x00800000
#define INPUT_MENU				0x01000000
#define INPUT_FASTFORWARD		0x02000000
#define INPUT_PAGEDOWN			0x04000000
#define INPUT_PAGEUP				0x08000000
#define INPUT_LOADSTATE			0x10000000
#define INPUT_SAVESTATE			0x20000000
#define INPUT_LOADDISKSTATE	0x40000000
#define INPUT_SAVEDISKSTATE	0x80000000

extern int joyx,joyy;	//zapper position
extern u8 joyzap;		//zapper trigger
extern u8 joykeys[];	//keyboard input

void input_init();
void input_kill();
void input_poll();

#endif
