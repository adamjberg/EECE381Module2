#ifndef BUTTON_H
#define BUTTON_H
#include "Global.h"

enum Type{menu = 0, action = 1, song = 2, none = 3};
struct Button{
	enum Type buttonType;
	struct Frame* mainPanel;
	struct Range* range;
	int type, bg_color;
	int x_pos, y_pos;
	char* name;
	struct Image* stats[2]; // show diff image when clicked
	void (*draw)(struct Button*);
	void (*collide) (struct Button*);
};

struct Button* initButton();
struct Button* initMenuButton(int, char*, int, int, struct Frame*);
struct Button* initSongButton(int, int, char*, int);
struct Button* initPlaylistButton(int, int, char*, int);
struct Button* initActionButton(int);
void drawMenuButton(struct Button*);
void drawSongButton(struct Button*);
void drawActionButton(struct Button*);
void dummyDraw(struct Button*);
void menuButtonCollide(struct Button*);
void playlistMenuButtonCollide(struct Button*);
void allSongsMenuButtonCollide(struct Button*);
void actionButtonCollide(struct Button*);
void playButtonCollide(struct Button*);
void dummyCollide(struct Button*);

// helper functions
int getXActionBtn(int);
int getYActionBtn(int);

#endif /* BUTTON_H_ */
