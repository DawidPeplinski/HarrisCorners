/*
 * fileMode.cpp
 *
 *  Created on: 16 Jan 2019
 *      Author: davca
 */

#include "fileMode.h"

FileMode::FileMode() {

}

FileMode::~FileMode() {

}

void FileMode::HandleMode() {
	std::cout << "Starting file mode.." << std::endl;
	std::cout << "Reading file from: " << this->filename << std::endl;
	Mat emptyWindow(500, 1000, CV_8UC3, Scalar(0,0, 100));
	namedWindow(MAIN_WINDOW_NAME);
	imshow(MAIN_WINDOW_NAME, emptyWindow);
	while(1) {
		this->frame = imread(this->filename, IMREAD_COLOR);
		if(!this->frame.data) {
			std::cerr << "Image file not found! Do you want to type the file's path? (y or n)" << std::endl;
			while(1) {
				int ans = waitKey(0);
				if(tolower(ans) == 'y') {
					std::system("clear");
					ReadPathFromKeyboard();
					break;
				} else if(tolower(ans) == 'n') {
					SetGlobalMode(MODE_EXIT);
					goto exit;
				}
			}
		} else {
			std::cout << "Image loaded" << std::endl;
			std::cout << "To switch to camera mode, press C." << std::endl;
			std::cout << "Use '[' and ']' to decrease/increase detection threshold" << std::endl;
			std::cout << "Press ESC to quit" << std::endl;
			while(1920 < this->frame.cols || 1080 < this->frame.rows)
				resize(this->frame, this->frame, Size(this->frame.cols/2, this->frame.rows/2));
			break;
		}
	}

	imshow(MAIN_WINDOW_NAME, this->frame);
	moveWindow(MAIN_WINDOW_NAME, 0, 0);
	this->harrisDetector.FindCorners(this->frame);
	while(GetGlobalMode() == MODE_FILE) {
		this->harrisDetector.ShowCorners();

		HandleKeyboard();
	}
exit:
	destroyAllWindows();
	std::cout << "Exiting file mode" << std::endl;
}

void FileMode::HandleKeyboard() {
	int key = waitKey(10);
	if(key == KEY_ESC)
		SetGlobalMode(MODE_EXIT);
	else if(key == KEY_CAMERA_MODE)
		SetGlobalMode(MODE_CAMERA);
	else if(key == KEY_INCREASE_DETECT_SENS) {
		harrisDetector.IncreaseSensivity();
		this->harrisDetector.FindCorners(this->frame);
	}
	else if(key == KEY_DECREASE_DETECT_SENS) {
		harrisDetector.DecreaseSensivity();
		this->harrisDetector.FindCorners(this->frame);
	}
}

/* Note: one or more opencv windows must be active for waitkey correct working */
void FileMode::ReadPathFromKeyboard() {
	std::string buf = "";
	while(1) {
		int key = waitKey(0);
		if(' ' <= key && key <= '~') {
			buf += (char)key;
		} else {
			switch(key) {
			case KEY_BACKSPACE:
				if(buf.length() > 0) {
					buf = buf.substr(0, buf.length() - 1);
				}
				break;

			case KEY_RETURN:
				this->filename = buf;
				return;

			default:
				/* overrun */
				break;
			}
		}
		std::system("clear");
		std::cout << buf << std::endl;
	}
}
