/*
 * main.h
 *
 *  Created on: 15 Jan 2019
 *      Author: davca
 */

#ifndef MAIN_H_
#define MAIN_H_

#define MAIN_WINDOW_NAME "MainWindow"

typedef enum {
	MODE_EXIT = 0,
	MODE_FILE = 1,
	MODE_CAMERA = 2
} mainModes_t;

void SetGlobalMode(mainModes_t mode);
mainModes_t GetGlobalMode();

#endif /* MAIN_H_ */
