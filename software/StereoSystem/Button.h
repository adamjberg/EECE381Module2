#ifndef BUTTON_H
#define BUTTON_H
#include "Global.h"

enum Type{menu = 0, action = 1, song = 2, others = 3};
struct Button{
	enum Type buttonType;
	struct Frame* Panel;
	struct Range* range;
	int id;
	int type, bg_color, frame;
	int x_pos, y_pos;
	int isClicked, startAnimate;
	char* name;
	struct Image* stats[2]; // show diff image when clicked
	void (*draw)(struct Button*);
	void (*collide) (struct Button*);
};

struct Button* initButton();
struct Button* initMenuButton(int, char*, int, struct Frame*);
struct Button* initSongButton(int, int, char*, int, struct Frame*);
struct Button* initPlaylistButton(int, int, char*, int, struct Frame*);
struct Button* initActionButton(int, struct Frame*);
struct Button* initScrollButton(int, int, int, struct Frame*);
struct Button* initVolumeButton(int, int, int, struct Frame*);

void drawTxtButton(struct Button*);
void drawActionButton(struct Button*);
void dummyDraw(struct Button*);

void menuButtonCollide(struct Button*);
void playlistMenuButtonCollide(struct Button*);
void allSongsMenuButtonCollide(struct Button*);

void actionButtonCollide(struct Button*);
void playButtonCollide(struct Button*);
void stopButtonCollide(struct Button*);
void pauseButtonCollide(struct Button*);
void prevButtonCollide(struct Button*);
void nextButtonCollide(struct Button*);
void volumeButtonCollide(struct Button*);
void dummyCollide(struct Button*);

void songButtonCollide(struct Button*);
void playlistButtonCollide(struct Button*);
void highlightButton(struct Button*);
void highlightSongWithID(int);

// helper functions
int getXActionBtn(int);
int getYActionBtn(int);
void drawRange(struct Button*);
void animateButton(struct Button*, int);
void updateVolumeValue(int);
struct Button* querySongButtonFromID(int);

void playSongsFromSongPanel(int, int, int);
void killSongButton(struct Button**);
#endif /* BUTTON_H_ */
