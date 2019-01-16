/*
 * cameraMode.h
 *
 *  Created on: 16 Jan 2019
 *      Author: davca
 */

#ifndef CAMERAMODE_H_
#define CAMERAMODE_H_

#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "main.h"

using namespace cv;

class CameraMode {
public:
	CameraMode();
	virtual ~CameraMode();
	void HandleMode();

private:
	typedef enum {
		KEY_ESC = 27,
		KEY_FILE_MODE = 'f',
		KEY_TOGGLE_CAPTURING = ' '
	} keys_t;
	Mat frame;
	bool capturingActive;
	void HandleKeyboard();
};

#endif /* CAMERAMODE_H_ */
