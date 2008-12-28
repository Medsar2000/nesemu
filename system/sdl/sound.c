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

#include <SDL/SDL.h>
#include "defines.h"
#include "system/config.h"

static int sound_bps = 16;
static int sound_samplerate = SOUND_HZ;
static int sound_fragsize = 1024;//SOUND_HZ / 60;
static void (*audio_callback)(void *buffer, int length) = 0;
static int soundinited = 0;

static void sdl_audio_player(void *udata, unsigned char *stream, int len)
{
	if(16 == sound_bps)
	   len /= 2;
	if(audio_callback)
	   audio_callback(stream,len);
}

void sound_init()
{
	SDL_AudioSpec wanted,obtained;
	unsigned int bufferSize;

	soundinited = 0;
	audio_callback = 0;
	if(config.soundenabled == 0)
		return;
	wanted.freq = SOUND_HZ;
	wanted.format = AUDIO_S16;
	wanted.channels = 1;
	wanted.samples = sound_fragsize;
	wanted.callback = sdl_audio_player;
	wanted.userdata = 0;
	if(SDL_OpenAudio(&wanted,&obtained) < 0) {
		log_message("Couldn't open audio: %s\n", SDL_GetError());
		return;
	}
	if(AUDIO_S16 != obtained.format) {
		log_message("Could not get correct audio output format\n");
		return;
	}
	sound_bps = (obtained.format == AUDIO_U8) ? 8 : 16;
	sound_samplerate = obtained.freq;
	bufferSize = (sound_bps / 8) * obtained.samples * 2;
	SDL_PauseAudio(0);
	soundinited = 1;
	log_message("sdl sound reset ok, %dhz, %d bits\n",sound_samplerate,sound_bps);
}

void sound_kill()
{
	if(soundinited)
		SDL_CloseAudio();
	soundinited = 0;
}

void sound_play()
{
	if(soundinited)
		SDL_PauseAudio(0);
}

void sound_pause()
{
	if(soundinited)
		SDL_PauseAudio(1);
}

void sound_setcallback(void (*cb)(void *buffer, int length))
{
	audio_callback = cb;
}

void sound_update()
{
}
