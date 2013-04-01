/*
 * Cursor.h
 *
 * Cursor class is a representation of hardware controller from the software side.  It
 * is used to handle selection and de-selection for all select-able objects inside the game.
 * An selected object may be return with cursor's coordinate for other purposes.
 * It does not process algorithm to handle placement of spaceship; however, it defines how UI
 * response to user.  An example will be that user hit the key on mouse to push a button on the 
 * Panel.  Cursor then notify the panel to response and create a spaceship (not place on the environment yet).  
 * As user hit the key again, it can pass the spaceship with x and y to other class which will
 * make decision if the tower should be placed or not.  The cursor will then notify the button to de-selected.
 *
 *  Created on: 2013-01-25
 *      Author: Danny
 */

#ifndef CURSOR_H_
#define CURSOR_H_
#include "Global.h"

struct Cursor {
	struct Object* super;
	struct Image* overlapImg;
	bool isLeftPressed, isRightPressed;
	struct Frame* frame;
};

struct Cursor* initCursor(int, int, struct Frame*);
void enableCursorInterrupt(struct Cursor*);
void memOverlap(struct Cursor*);
void updateCursor(struct Cursor*);
int getCursorX(struct Cursor*);
int getCursorY(struct Cursor*);
void moveCursor(struct Cursor*, int, int);
/*
void setPanelToCursor(struct Cursor*, struct Frame*);

void collideCursor(void*, void*, struct Collidable*);

void dragOverlap(struct Cursor*);
void cursorAnimate(struct animation**, int, int, int, int, struct Object*, struct Cursor*);

void addButton(struct Cursor*, struct Button*);
void checkButtonCollision(struct Cursor*);
void setFraneToCursor(struct Cursor*, struct Frame*);
void clearPanelFromCursor(struct Cursor*);*/
#endif /* CURSOR_H_ */
