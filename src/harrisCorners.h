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
	void DestroyCornersWindow();
	void IncreaseSensivity() { this->threshold = (this->maxThreshold < (this->threshold + 5)) ? this->maxThreshold : this->threshold + 5; }
	void DecreaseSensivity() { this->threshold = ((this->threshold - 5) < this->minThreshold) ? this->minThreshold : this->threshold - 5; }
	void IncreaseBlocksize() { this->blocksize = (this->maxBlocksize < (this->blocksize + 1)) ? this->maxBlocksize : this->blocksize + 1; }
	void DecreaseBlocksize() { this->blocksize = ((this->blocksize - 1) < this->minBlocksize) ? this->minBlocksize : this->blocksize - 1; }

private:
	static const int maxThreshold = 255;
	static const int minThreshold = 0;
	static const int maxBlocksize = 16;
	static const int minBlocksize = 2;
	static const int maxPointsCount = 100000;
	int threshold;
	int blocksize;
	Point pointsTab[maxPointsCount];
	int currentPointsCount;
	Mat result;
};

#endif /* HARRISCORNERS_H_ */
