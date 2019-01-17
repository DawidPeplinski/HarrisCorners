/*
 * harrisCorners.cpp
 *
 *  Created on: 16 Jan 2019
 *      Author: davca
 */

#include "harrisCorners.h"
#include <iostream>

HarrisCorners::HarrisCorners() {
	this->threshold = this->maxThreshold;
	this->blocksize = 4;
	this->currentPointsCount = 0;
}

HarrisCorners::~HarrisCorners() {

}

void HarrisCorners::FindCorners(Mat frame) {
	Mat dst, dst_norm, gray_frame;
	cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
	dst = Mat::zeros(frame.size(), CV_32FC1);
	int apertureSize = 3;
	double k = 0.04;
	// Detecting corners
	cornerHarris(gray_frame, dst, this->blocksize, apertureSize, k, BORDER_DEFAULT);
	// Normalizing
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, this->result);
	// Counting thresholded points to prevent program seizing during circles drawing
	int count = 0;
	std::cout << "Current threshold: " << this->threshold << std::endl;
	std::cout << "Current blocksize: " << this->blocksize << std::endl;
	for(int j = 0; j < dst_norm.rows; j++) {
		for(int i = 0; i < dst_norm.cols; i++) {
			if((int)dst_norm.at<float>(j,i) > this->threshold) {
			   this->pointsTab[count++] = Point(i, j);
			   if(this->maxPointsCount <= count) {
				   this->currentPointsCount = 0;
				   std::cout << "Too many points detected!" << std::endl;
				   return;
			   }
			}
		}
	}
	// Drawing rectangles around corners
	this->currentPointsCount = count;
	for(int i = 0; i < this->currentPointsCount; i++) {
		Point p = this->pointsTab[i];
		rectangle(this->result, Point(p.x - this->blocksize, p.y - this->blocksize), Point(p.x + this->blocksize, p.y + this->blocksize), Scalar(0), 2, LINE_4, 0);
	}
	std::cout << "Found points count: " << this->currentPointsCount << std::endl;
}

void HarrisCorners::ShowCorners() {
	// Showing the result
	imshow(HARRIS_CORNERS_WINDOW, this->result);
}

void HarrisCorners::DestroyCornersWindow() {
	destroyWindow(HARRIS_CORNERS_WINDOW);
	this->currentPointsCount = 0;
}

void HarrisCorners::MouseCallback(int  event, int  x, int  y, int  flag, void *param) {
	HarrisCorners *m = (HarrisCorners *)param;
	if(event == EVENT_LBUTTONUP) {
		for(int i = 0; i < m->currentPointsCount; i++) {
			Point p = m->pointsTab[i];
			if(abs(p.x - x) < m->blocksize && abs(p.y - y) < m->blocksize) {
				rectangle(m->result, Point(p.x - m->blocksize, p.y - m->blocksize), Point(p.x + m->blocksize, p.y + m->blocksize), Scalar(255), 2, LINE_4, 0);
				m->ShowCorners();
				break;
			}
		}
	}
}

void HarrisCorners::BindMouseCallback() {
	setMouseCallback(HARRIS_CORNERS_WINDOW, MouseCallback, this);
}
