/*
 * fileMode.h
 *
 *  Created on: 16 Jan 2019
 *      Author: davca
 */

#ifndef FILEMODE_H_
#define FILEMODE_H_

#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "harrisCorners.h"
#include "main.h"

using namespace cv;

class FileMode {
public:
	FileMode();
	virtual ~FileMode();
	void HandleMode();
	void SetFilename(std::string filename) { this->filename = filename; }

private:
	typedef enum {
		KEY_RETURN = 13,
		KEY_BACKSPACE = 8,
		KEY_ESC = 27,
		KEY_CAMERA_MODE = 'c',
		KEY_INCREASE_DETECT_THRES = ']',
		KEY_DECREASE_DETECT_THRES = '[',
		KEY_INCREASE_BLOCKSIZE = '.',
		KEY_DECREASE_BLOCKSIZE = ','
	} keys_t;
	std::string filename;
	Mat frame;
	HarrisCorners harrisDetector;
	void HandleKeyboard();
	void ReadPathFromKeyboard();
};

#endif /* FILEMODE_H_ */
