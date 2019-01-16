//============================================================================
// Name        : POWR.cpp
// Author      : dpeplinski
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include "cameraMode.h"
#include "fileMode.h"
#include "main.h"

using namespace cv;
mainModes_t globalMode;

void SetGlobalMode(mainModes_t mode) { globalMode = mode; }
mainModes_t GetGlobalMode() { return globalMode; }

static mainModes_t parseInputArguments(int argc, char **argv, std::string &filename) {
	char *ptr = NULL;
	mainModes_t mode = MODE_EXIT;
	for(int i = 1; i < argc; i++) {
		ptr = strchr(argv[i], '-');
		/* '-' symbol found, parse the flag */
		if(ptr != NULL) {
			switch(*(ptr + 1)) {
				case 'f':
					mode = MODE_FILE;
					/* if there is another argument provided */
					if((i + 1) < argc)
						filename = argv[i + 1];
					break;

				case 'c':
					mode = MODE_CAMERA;
					break;

				default:
					std::cerr << "Unrecognized parameter." << std::endl;
			}
		}
	}
	return mode;
}

int main(int argc, char **argv) {
	CameraMode cameraMode;
	FileMode fileMode;
	std::string filename = "";
	globalMode = parseInputArguments(argc, argv, filename);
	fileMode.SetFilename(filename);
	if(globalMode == MODE_EXIT) {
		std::cout << "Choose one from available modes: -f for file mode, -c for camera mode." << std::endl;
		std::cout << "When you use file mode, you can type the file's path after -f flag" << std::endl;
	}
	while(1) {
		switch(globalMode) {
			case MODE_FILE:
				fileMode.HandleMode();
				break;

			case MODE_CAMERA:
				cameraMode.HandleMode();
				break;

			case MODE_EXIT:
				return 0;

			default:
				return -1;
		}
	}
}
