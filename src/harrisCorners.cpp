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
}

HarrisCorners::~HarrisCorners() {

}

void HarrisCorners::FindCorners(Mat frame) {
	Mat dst, dst_norm, gray_frame;
	cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
	dst = Mat::zeros(frame.size(), CV_32FC1);
	// Detector parameters
	int blockSize = 4;
	int apertureSize = 3;
	double k = 0.04;
	// Detecting corners
	cornerHarris(gray_frame, dst, blockSize, apertureSize, k, BORDER_DEFAULT);
	// Normalizing
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, this->result);
	// Counting thresholded points to prevent program seizing during circles drawing
	int count = 0;
	std::cout << "Current threshold: " << this->threshold << std::endl;
	for(int j = 0; j < dst_norm.rows; j++) {
		for(int i = 0; i < dst_norm.cols; i++) {
			if((int)dst_norm.at<float>(j,i) > this->threshold) {
			   count++;
			}
		}
	}
	// Drawing a circle around corners
	if(count < 100000) {
		for(int j = 0; j < dst_norm.rows; j++) {
			for(int i = 0; i < dst_norm.cols; i++) {
				if((int)dst_norm.at<float>(j,i) > this->threshold) {
				   circle(this->result, Point(i, j), 8, Scalar(0), 2, LINE_8, 0);
				}
			}
		}
	} else {
		std::cout << "Too many points detected!" << std::endl;
	}

}

void HarrisCorners::ShowCorners() {
	// Showing the result
	imshow(HARRIS_CORNERS_WINDOW, this->result);
}
