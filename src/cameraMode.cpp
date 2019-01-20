/*
 * cameraMode.cpp
 *
 *  Created on: 16 Jan 2019
 *      Author: davca
 */

#include "cameraMode.h"

CameraMode::CameraMode() {
	this->capturingActive = true;
	this->histMode = 1;
}

CameraMode::~CameraMode() {

}

void CameraMode::HandleMode() {
	std::system("clear");
	std::cout << "Starting camera mode.." << std::endl;
	Mat emptyWindow(500, 1000, CV_8UC3, Scalar(0,0, 100));
	VideoCapture capture;
	if(!capture.open(0)) {
		SetGlobalMode(MODE_EXIT);
		std::cerr << "Camera not found!" << std::endl;
		goto exit;
	} else {
		std::cout << "Press SPACE to start/stop capturing. To switch to file mode, press '" << (char)KEY_FILE_MODE << "'" << std::endl;
		std::cout << "Use '" << (char)KEY_DECREASE_DETECT_THRES << "' and '" << (char)KEY_INCREASE_DETECT_THRES << "' to decrease/increase detection threshold when capturing is stopped" << std::endl;
		std::cout << "Use '" << (char)KEY_DECREASE_BLOCKSIZE << "' and '" << (char)KEY_INCREASE_BLOCKSIZE << "' to decrease/increase blocksize" << std::endl;
		std::cout << "Program by default is working in mode '1': selecting a point with mouse click shows a histogram for the selected pixels block" << std::endl;
		std::cout << "Mode '2' calculates histograms of all points, compares them and shows similar points" << std::endl;
		std::cout << "Sensitivity in mode '2' is decreased/increased with '" << (char)KEY_DECREASE_HISTCOMP_SENS << "' and '" << (char)KEY_INCREASE_HISTCOMP_SENS << "'" << std::endl;
		std::cout << "Press '" << (char)KEY_SWITCH_HIST_MODES << "' to switch between modes" << std::endl;
		std::cout << "Press ESC to quit" << std::endl;
	}
	namedWindow(MAIN_WINDOW_NAME);
	imshow(MAIN_WINDOW_NAME, emptyWindow);
	moveWindow(MAIN_WINDOW_NAME, 0, 0);
	while(GetGlobalMode() == MODE_CAMERA) {
		if(this->capturingActive) {
			capture >> this->frame;
			imshow(MAIN_WINDOW_NAME, this->frame);
		} else {
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
		}
		HandleKeyboard();
	}
exit:
	destroyAllWindows();
	std::cout << "Exiting camera mode" << std::endl;
}

void CameraMode::HandleKeyboard() {
	int delay = (this->capturingActive) ? 10 : 0;
	int key = waitKey(delay);
	if(key == KEY_ESC)
		SetGlobalMode(MODE_EXIT);
	else if(key == KEY_FILE_MODE) {
		SetGlobalMode(MODE_FILE);
	} else if(key == KEY_TOGGLE_CAPTURING) {
		this->capturingActive = !this->capturingActive;
		if(this->capturingActive == true) {
			this->cornersDetector.DestroyCornersWindow();
		}
	} else if(key == KEY_INCREASE_DETECT_THRES && this->capturingActive == false) {
		this->cornersDetector.IncreaseSensivity();
	} else if(key == KEY_DECREASE_DETECT_THRES && this->capturingActive == false) {
		cornersDetector.DecreaseSensivity();
	} else if(key == KEY_INCREASE_BLOCKSIZE && this->capturingActive == false) {
		this->cornersDetector.IncreaseBlocksize();
	} else if(key == KEY_DECREASE_BLOCKSIZE && this->capturingActive == false) {
		cornersDetector.DecreaseBlocksize();
	} else if(key == KEY_INCREASE_HISTCOMP_SENS && this->histMode == 2 && this->capturingActive == false) {
		this->cornersDetector.IncreaseHistCompSensivity();
	} else if(key == KEY_DECREASE_HISTCOMP_SENS && this->histMode == 2 && this->capturingActive == false) {
		this->cornersDetector.DecreaseHistCompSensivity();
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
