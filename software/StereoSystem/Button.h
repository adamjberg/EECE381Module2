#ifndef BUTTON_H
#define BUTTON_H
#include "Global.h"

enum Type{menu = 0, action = 1, song = 2, none = 3};
struct Button{
	enum Type buttonType;
	struct Frame* Panel;
	struct Range* range;
	int type, bg_color;
	int x_pos, y_pos;
	int isClicked;
	char* name;
	struct Image* stats[2]; // show diff image when clicked
	void (*draw)(struct Button*);
	void (*collide) (struct Button*);
};

struct Button* initButton();
struct Button* initMenuButton(int, char*, int, struct Frame*);
struct Button* initSongButton(int, int, char*, struct Frame*);
struct Button* initPlaylistButton(int, int, char*, struct Frame*);
struct Button* initActionButton(int);

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
void dummyCollide(struct Button*);

void songButtonCollide(struct Button*);
void playlistButtonCollide(struct Button*);

// helper functions
int getXActionBtn(int);
int getYActionBtn(int);
void drawRange(struct Button*);

#endif /* BUTTON_H_ */
