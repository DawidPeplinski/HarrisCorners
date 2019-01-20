/*
 * fileMode.cpp
 *
 *  Created on: 16 Jan 2019
 *      Author: davca
 */

#include "fileMode.h"

FileMode::FileMode() {
	this->histMode = 1;
}

FileMode::~FileMode() {

}

void FileMode::HandleMode() {
	std::system("clear");
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
			std::cout << "To switch to camera mode, press '" << (char)KEY_CAMERA_MODE << "'" << std::endl;
			std::cout << "There are two detector modes: Harris and Moravec" << std::endl;
			std::cout << "Press '" << (char)KEY_SWITCH_DETECT_MODES << "' to switch between detect modes" << std::endl;
			std::cout << "Use '" << (char)KEY_DECREASE_DETECT_THRES << "' and '" << (char)KEY_INCREASE_DETECT_THRES << "' to decrease/increase detection threshold" << std::endl;
			std::cout << "Use '" << (char)KEY_DECREASE_BLOCKSIZE << "' and '" << (char)KEY_INCREASE_BLOCKSIZE << "' to decrease/increase blocksize" << std::endl;
			std::cout << "Program by default is working in histogram mode '1': selecting a point with mouse click shows a histogram for the selected pixels block" << std::endl;
			std::cout << "Mode '2' calculates histograms of all points, compares them and shows similar points" << std::endl;
			std::cout << "Sensitivity in mode '2' is decreased/increased with '" << (char)KEY_DECREASE_HISTCOMP_SENS << "' and '" << (char)KEY_INCREASE_HISTCOMP_SENS << "'" << std::endl;
			std::cout << "Press '" << (char)KEY_SWITCH_HIST_MODES << "' to switch between histogram modes" << std::endl;
			std::cout << "Press ESC to quit" << std::endl;
			while(1920 < this->frame.cols || 1080 < this->frame.rows)
				resize(this->frame, this->frame, Size(this->frame.cols/2, this->frame.rows/2));
			break;
		}
	}
	namedWindow(MAIN_WINDOW_NAME);
	imshow(MAIN_WINDOW_NAME, this->frame);
	moveWindow(MAIN_WINDOW_NAME, 0, 0);
	while(GetGlobalMode() == MODE_FILE) {
		switch(this->histMode) {
			case 1:
				this->cornersDetector.FindCorners(this->frame);
				this->cornersDetector.BindMouseCallback();
				imshow(MAIN_WINDOW_NAME, this->frame);
				break;

			case 2:
				this->cornersDetector.CompareCorners(this->frame);
				this->cornersDetector.UnbindMouseCallback();
				break;
			default:
				// overrun
				break;
		}
		HandleKeyboard();
	}
exit:
	destroyAllWindows();
	std::cout << "Exiting file mode" << std::endl;
}

void FileMode::HandleKeyboard() {
	int key = waitKey(0);
	if(key == KEY_ESC)
		SetGlobalMode(MODE_EXIT);
	else if(key == KEY_CAMERA_MODE)
		SetGlobalMode(MODE_CAMERA);
	else if(key == KEY_INCREASE_DETECT_THRES) {
		cornersDetector.IncreaseSensivity();
	} else if(key == KEY_DECREASE_DETECT_THRES) {
		cornersDetector.DecreaseSensivity();
	} else if(key == KEY_INCREASE_BLOCKSIZE) {
		this->cornersDetector.IncreaseBlocksize();
	} else if(key == KEY_DECREASE_BLOCKSIZE) {
		cornersDetector.DecreaseBlocksize();
	} else if(key == KEY_INCREASE_HISTCOMP_SENS && this->histMode == 2) {
		this->cornersDetector.IncreaseHistCompSensivity();
	} else if(key == KEY_DECREASE_HISTCOMP_SENS && this->histMode == 2) {
		this->cornersDetector.DecreaseHistCompSensivity();
	} else if(key == KEY_SWITCH_DETECT_MODES) {
		system("clear");
		this->cornersDetector.SwitchDetectorMode();
	} else if(key == KEY_SWITCH_HIST_MODES) {
		if(this->histMode == 1) {
			system("clear");
			std::cout << "Switched to mode '2'" << std::endl;
			this->histMode = 2;
		} else {
			system("clear");
			std::cout << "Switched to mode '1'" << std::endl;
			this->histMode = 1;
		}
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
