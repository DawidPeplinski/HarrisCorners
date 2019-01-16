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
		KEY_CAMERA_MODE = 'c'
	} keys_t;
	std::string filename;
	Mat frame;
	void HandleKeyboard();
	void ReadPathFromKeyboard();
};

#endif /* FILEMODE_H_ */
