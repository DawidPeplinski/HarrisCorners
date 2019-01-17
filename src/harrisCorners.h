/*
 * harrisCorners.h
 *
 *  Created on: 16 Jan 2019
 *      Author: davca
 */

#ifndef HARRISCORNERS_H_
#define HARRISCORNERS_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "main.h"

using namespace cv;

class HarrisCorners {
public:
	HarrisCorners();
	virtual ~HarrisCorners();
	void FindCorners(Mat frame);
	void ShowCorners();
	void IncreaseSensivity() { this->threshold = (this->maxThreshold < (this->threshold + 5)) ? this->maxThreshold : this->threshold + 5; }
	void DecreaseSensivity() { this->threshold = ((this->threshold - 5) < 0) ? 0 : this->threshold - 5; }

private:
	const int maxThreshold = 255;
	int threshold;
	Mat result;
};

#endif /* HARRISCORNERS_H_ */
