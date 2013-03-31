/*
 * Global.h
 *
 *  Created on: 2013-03-02
 *      Author: danny
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "system.h"

#include "altera_up_avalon_rs232.h"
#include "altera_up_avalon_audio.h"
#include <alt_types.h>
#include <altera_up_sd_card_avalon_interface.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_ps2.h"

#include "sys/alt_stdio.h"
#include "sys/alt_irq.h"
#include "sys/alt_alarm.h"

#include "ISR_vectors.h"
#include "RS232.h"
#include "Queue.h"
#include "Packet.h"
#include "Object.h"
#include "Coder.h"
#include "Playlist.h"
#include "Database.h"
#include "Command.h"
#include "Song.h"
#include "TestBot.h"
#include "CommandScheduler.h"
#include "sound/SoundMixer.h"
#include "sound/Sound.h"
#include "sound/AudioDeviceController.h"
#include "PushKeyController.h"
#include "Graphic.h"
#include "MemoryMgr.h"
#include "Range.h"
#include "Cursor.h"
#include "Button.h"
#include "Frame.h"

#define HEADERSIZE 2
#define SPACE 32
#define NEWLINE 10

extern alt_alarm alarm;
extern struct alt_up_dev up_dev;
extern alt_up_pixel_buffer_dma_dev* pixel_buffer;
extern alt_up_char_buffer_dev* char_buffer;
extern struct RS232 com;
extern int queue_lock;
extern struct database db;
extern int song_id_lock;
extern int SDIO_lock;
extern int MIX_LOCK;
//extern int cursor_lock;
extern struct MemoryMgr memMgr;
extern struct Cursor* mouse;

enum msgType {
	STRING=0, CMD=1, PLAYLIST=2, AUDIO=3
};
#endif /* GLOBAL_H_ */
