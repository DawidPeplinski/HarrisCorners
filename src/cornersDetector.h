/*
 * cornersDetector.h
 *
 *  Created on: 16 Jan 2019
 *      Author: davca
 */

#ifndef CORNERSDETECTOR_H_
#define CORNERSDETECTOR_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "main.h"

#define CORNERS_WINDOW "Corners"
#define HISTOGRAM_WINDOW "Gray Histogram"

using namespace cv;

class CornersDetector {
public:
	CornersDetector();
	virtual ~CornersDetector();
	void FindCorners(Mat frame);
	void CompareCorners(Mat frame);
	void DestroyCornersWindow();
	void BindMouseCallback();
	void UnbindMouseCallback();
	void IncreaseSensivity() { this->threshold = (this->maxThreshold < (this->threshold + 5)) ? this->maxThreshold : this->threshold + 5; }
	void DecreaseSensivity() { this->threshold = ((this->threshold - 5) < this->minThreshold) ? this->minThreshold : this->threshold - 5; }
	void IncreaseBlocksize() { this->blocksize = (this->maxBlocksize < (this->blocksize + 1)) ? this->maxBlocksize : this->blocksize + 1; }
	void DecreaseBlocksize() { this->blocksize = ((this->blocksize - 1) < this->minBlocksize) ? this->minBlocksize : this->blocksize - 1; }
	void IncreaseHistCompSensivity() { this->histCompSensitivity = (this->maxHistCompSens < (this->histCompSensitivity + 0.1f)) ? this->maxHistCompSens : this->histCompSensitivity + 0.1f; }
	void DecreaseHistCompSensivity() { this->histCompSensitivity = ((this->histCompSensitivity - 0.1f) < this->minHistCompSens) ? this->minHistCompSens : this->histCompSensitivity - 0.1f; }
	void SwitchDetectorMode() { this->detectorMode = (this->detectorMode == DETECTOR_HARRIS_MODE) ? DETECTOR_MORAVEC_MODE : DETECTOR_HARRIS_MODE; }

private:
	void HarrisCorners(Mat &frame);
	void MoravecCorners(Mat &frame);
	static void MouseCallback(int event, int x, int y, int flag, void *param);
	void CalculateHistogram(Mat& frame, Mat& hist, bool ifShow);
	void DrawRectangle(Mat &frame, Point p, Scalar color);
	typedef enum {
		DETECTOR_HARRIS_MODE,
		DETECTOR_MORAVEC_MODE
	} detectorModes_t;
	const int maxThreshold = 255;
	const int minThreshold = 5;
	const int maxBlocksize = 16;
	const int minBlocksize = 2;
	const float maxHistCompSens = 0.9f;
	const float minHistCompSens = 0.1f;
	static const int maxPointsCount = 100000;
	detectorModes_t detectorMode;
	int threshold;
	int blocksize;
	int histMode;
	float histCompSensitivity;
	std::vector<Point> pointsTab;
	int currentPointsCount;
	Mat source;
	Mat result;
};

#endif /* CORNERSDETECTOR_H_ */
