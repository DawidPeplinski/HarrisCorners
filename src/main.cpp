//============================================================================
// Name        : POWR.cpp
// Author      : dpeplinski
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/plot.hpp>
#include <iostream>
#include <string>
#include "main.h"
using namespace cv;

static struct {
	bool isUsed;
	std::string filename;
} fileMode;

static struct {
	bool isUsed;
} cameraMode;

static struct {
	Mat frame;
} frameHandling;

static void parseInputArguments(int argc, char **argv) {
	char *ptr = NULL;
	for(int i = 1; i < argc; i++) {
		ptr = strchr(argv[i], '-');
		/* '-' symbol found, parse the flag */
		if(ptr != NULL) {
			switch(*(ptr + 1)) {
				case 'f':
					fileMode.isUsed = true;
					/* if there is another argument provided */
					if((i + 1) < argc)
						fileMode.filename = argv[i + 1];
					break;

				case 'c':
					cameraMode.isUsed = true;
					break;

				default:
					std::cerr << "Unrecognized parameter." << std::endl;
					goto invalid;
			}
		}
	}
	if(fileMode.isUsed == false && cameraMode.isUsed == false)
		goto invalid;
	else
		return;
invalid:
	std::cout << "Choose one from available modes: -f for file mode, -c for camera mode." << std::endl;
	std::cout << "When you use file mode, you can type the file's path after -f flag" << std::endl;
}

/* Note: one or more opencv windows must be active for waitkey correct working */
static void readFromKeyboard(String &buf) {
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

void handleFileMode() {
	std::cout << "Starting file mode.." << std::endl;
	std::cout << "Reading file from: " << fileMode.filename << std::endl;
	Mat emptyWindow(500, 1000, CV_8UC3, Scalar(0,0, 100));
	namedWindow("Raw");
	imshow("Raw", emptyWindow);
	while(1) {
		frameHandling.frame = imread(fileMode.filename, IMREAD_COLOR);
		if(!frameHandling.frame.data) {
			std::cerr << "Image file not found! Do you want to type the file's path? (y or n)" << std::endl;
			while(1) {
				int ans = waitKey(0);
				if(tolower(ans) == 'y') {
					fileMode.filename = "";
					std::system("clear");
					readFromKeyboard(fileMode.filename);
					break;
				} else if(tolower(ans) == 'n') {
					fileMode.isUsed = false;
					goto exit;
				}
			}
		} else {
			std::cout << "Image loaded." << std::endl;
			break;
		}
	}

	imshow("Raw", frameHandling.frame);
	while(fileMode.isUsed) {

		if(waitKey(10) == KEY_ESC)
			fileMode.isUsed = false;
	}
exit:
	std::cout << "Exiting file mode." << std::endl;
}

void handleCameraMode() {
	std::cout << "Starting camera mode.." << std::endl;
	Mat emptyWindow(500, 1000, CV_8UC3, Scalar(0,0, 100));
	namedWindow("Raw");
	imshow("Raw", emptyWindow);
	while(cameraMode.isUsed) {

		if(waitKey(10) == KEY_ESC)
			cameraMode.isUsed = false;
	}
	std::cout << "Exiting camera mode." << std::endl;
}

int main(int argc, char **argv) {
	parseInputArguments(argc, argv);
	while(1) {
		if(fileMode.isUsed && cameraMode.isUsed) {
			std::cerr << "Choose only one mode." << std::endl;
			break;
		} else if(fileMode.isUsed)
			handleFileMode();
		else if(cameraMode.isUsed)
			handleCameraMode();
		else
			break;
	}
	return 0;
}
