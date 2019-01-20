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

#include "cornersDetector.h"
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
		KEY_TOGGLE_CAPTURING = ' ',
		KEY_INCREASE_DETECT_THRES = ']',
		KEY_DECREASE_DETECT_THRES = '[',
		KEY_INCREASE_BLOCKSIZE = '\'',
		KEY_DECREASE_BLOCKSIZE = ';',
		KEY_SWITCH_HIST_MODES = 'm',
		KEY_INCREASE_HISTCOMP_SENS = '.',
		KEY_DECREASE_HISTCOMP_SENS = ','
	} keys_t;
	Mat frame;
	CornersDetector cornersDetector;
	bool capturingActive;
	int histMode;
	void HandleKeyboard();
};

#endif /* CAMERAMODE_H_ */
