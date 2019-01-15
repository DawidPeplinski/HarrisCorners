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
using namespace cv;

static struct {
	bool isUsed;
	char filename[256];
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
						strcpy(fileMode.filename, argv[i + 1]);
					break;

				case 'c':
					cameraMode.isUsed = true;
					break;

				default:
					std::cerr << "Unrecognized parameter." << std::endl;
					std::cerr << "Choose one from available modes: -f for file mode, -c for camera mode." << std::endl;
			}
		}
	}
	if(ptr == NULL)
		std::cerr << "Choose one from available modes: -f for file mode, -c for camera mode." << std::endl;
}

void handleFileMode() {
	std::cout << "Starting file mode.." << std::endl;
	std::cout << "Reading file from: " << fileMode.filename << std::endl;
	frameHandling.frame = imread(fileMode.filename, IMREAD_COLOR);
	if(!frameHandling.frame.data) {
		std::cerr << "Image file not found!" << std::endl;
		return;
	}
	namedWindow("Raw");
	imshow("Raw", frameHandling.frame);
	while(fileMode.isUsed) {

		if(waitKey(1) == 27)
			fileMode.isUsed = false;
	}
}

void handleCameraMode() {
	std::cout << "Starting camera mode.." << std::endl;

	while(cameraMode.isUsed) {

		if(waitKey(1) == 27)
			cameraMode.isUsed = false;
	}
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
