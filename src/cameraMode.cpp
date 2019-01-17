/*
 * cameraMode.cpp
 *
 *  Created on: 16 Jan 2019
 *      Author: davca
 */

#include "cameraMode.h"

CameraMode::CameraMode() {
	capturingActive = true;
}

CameraMode::~CameraMode() {

}

void CameraMode::HandleMode() {
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
		std::cout << "Press ESC to quit" << std::endl;
	}
	namedWindow(MAIN_WINDOW_NAME);
	imshow(MAIN_WINDOW_NAME, emptyWindow);
	moveWindow(MAIN_WINDOW_NAME, 0, 0);
	while(GetGlobalMode() == MODE_CAMERA) {
		if(this->capturingActive)
			capture >> this->frame;
		else
			this->harrisDetector.ShowCorners();
		imshow(MAIN_WINDOW_NAME, this->frame);

		HandleKeyboard();
	}
exit:
	destroyAllWindows();
	std::cout << "Exiting camera mode" << std::endl;
}

void CameraMode::HandleKeyboard() {
	int key = waitKey(10);
	if(key == KEY_ESC)
		SetGlobalMode(MODE_EXIT);
	else if(key == KEY_FILE_MODE) {
		SetGlobalMode(MODE_FILE);
	} else if(key == KEY_TOGGLE_CAPTURING) {
		this->capturingActive = !this->capturingActive;
		if(this->capturingActive == false) {
			this->harrisDetector.FindCorners(this->frame);
			this->harrisDetector.ShowCorners();
			this->harrisDetector.BindMouseCallback();
		} else {
			this->harrisDetector.DestroyCornersWindow();
		}
	} else if(key == KEY_INCREASE_DETECT_THRES && this->capturingActive == false) {
		this->harrisDetector.IncreaseSensivity();
		this->harrisDetector.FindCorners(this->frame);
	} else if(key == KEY_DECREASE_DETECT_THRES && this->capturingActive == false) {
		harrisDetector.DecreaseSensivity();
		this->harrisDetector.FindCorners(this->frame);
	} else if(key == KEY_INCREASE_BLOCKSIZE && this->capturingActive == false) {
		this->harrisDetector.IncreaseBlocksize();
		this->harrisDetector.FindCorners(this->frame);
	} else if(key == KEY_DECREASE_BLOCKSIZE && this->capturingActive == false) {
		harrisDetector.DecreaseBlocksize();
		this->harrisDetector.FindCorners(this->frame);
	}
}
