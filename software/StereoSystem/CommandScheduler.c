/*
 * CommandScheduler.c
 *
 *  Created on: 2013-03-08
 *      Author: danny
 */

#include "CommandScheduler.h"

struct CmdScheduler* initCmdScheduler() {
	struct CmdScheduler* this = (struct CmdScheduler*)malloc(sizeof(struct CmdScheduler));
	this->cmds = initQueue();
	return this;
}

/*
 * This function schedule a command in the waiting list to be executed
 */
void cmdProcessing(struct CmdScheduler* this) {

}
