#ifndef UI_H_
#define UI_H_
#include "Global.h"

#define POSITION 20
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

struct Frame{
	struct Frame** elements;
	struct Button** buttons;
	int element_size;

	void (*drawFrame)(struct Frame*);
};

struct Frame* initFrame();
struct Frame* initMainFrame();
struct Frame* initMenuFrame();
struct Frame* initActionFrame();
struct Frame* initSongPanel();
void drawMainFrame(struct Frame* this);
void drawMenuFrame(struct Frame* this);
void drawActionFrame(struct Frame* this);
void drawSongPanel(struct Frame* this);
void getSongs();
void getPlaylists();
void displayLoadingScreenVGA();

#endif /* FRAME_H_ */
