#ifndef UI_H_
#define UI_H_
#include "Global.h"

#define SONG_Y_POSITION 4
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

struct Frame{
	struct Frame** elements;
	struct Button** buttons;
	int element_size, button_size;
	int currentPanel, currentPlaylist; // signify song / list being shown
	struct Image* bg_image;
	struct Frame* mainFrame;


	void (*drawFrame)(struct Frame*);
};

struct Frame* initFrame();
struct Frame* initMainFrame();
struct Frame* initMenuFrame(struct Frame*);
struct Frame* initActionFrame(struct Frame*);
struct Frame* initSongPanel(struct Frame*);
struct Frame* initPlaylistPanel(struct Frame*);
struct Frame* initScrollFrame(struct Frame*);
struct Frame* initVolumeFrame(struct Frame*);
struct Frame* initSongInListPanel(struct Frame*, int);

void drawMainFrame(struct Frame*);
void drawMenuFrame(struct Frame*);
void drawActionFrame(struct Frame*);
void drawSongPanel(struct Frame*);
void drawPlaylistPanel(struct Frame*);
void drawScrollFrame(struct Frame*);
void drawVolumeFrame(struct Frame*);
void drawSongInListPanel(struct Frame*);
void clearSongPanel();
void displayLoadingScreenVGA();

void drawAllSongs();
void drawAllLists();
void drawAllSongsInList(int);

void killSongPanel(struct Frame**);
void killSongInListPanel(struct Frame**);
#endif /* FRAME_H_ */
