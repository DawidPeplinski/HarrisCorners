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
	this->source = frame.clone();
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
		Point p1 = Point(p.x - this->blocksize, p.y - this->blocksize);
		Point p2 = Point(p.x + this->blocksize, p.y + this->blocksize);
		rectangle(this->result, p1, p2, Scalar(0), 2, LINE_4, 0);
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

void HarrisCorners::CalculateHistogram(Mat& frame) {
	cvtColor(frame, frame, COLOR_BGR2GRAY);
    // Set histogram bins count
    int bins = 256;
    int histSize[] = { bins };
    // Set ranges for histogram bins
    float lranges[] = { 0, 256 };
    const float* ranges[] = { lranges };
    // create matrix for histogram
    Mat hist;
    int channels[] = { 0 };
    // create matrix for histogram visualization
    int const hist_height = bins;
    Mat3b hist_image = Mat3b::zeros(hist_height, bins);
    // calculate histogram
    calcHist(&frame, 1, channels, cv::Mat(), hist, 1, histSize, ranges, true, false);

    double max_val = 0;
    minMaxLoc(hist, 0, &max_val);

    // visualize each bin
    for(int b = 0; b < bins; b++) {
        float const binVal = hist.at<float>(b);
        int   const height = cvRound(binVal*hist_height/max_val);
        line
            (hist_image,
            Point(b, hist_height-height),
			Point(b, hist_height),
            Scalar::all(255)
            );
    }
    // Workaround for checking if window exists
    if(getWindowProperty(HISTOGRAM_WINDOW, WND_PROP_AUTOSIZE) == 1)
    	destroyWindow(HISTOGRAM_WINDOW);
    namedWindow(HISTOGRAM_WINDOW, WINDOW_AUTOSIZE);
    imshow(HISTOGRAM_WINDOW, hist_image);
}

void HarrisCorners::MouseCallback(int  event, int  x, int  y, int  flag, void *param) {
	HarrisCorners *m = (HarrisCorners *)param;
	if(event == EVENT_LBUTTONUP) {
		for(int i = 0; i < m->currentPointsCount; i++) {
			Point p = m->pointsTab[i];
			if(abs(p.x - x) < m->blocksize && abs(p.y - y) < m->blocksize) {
				// Painting selected rectangle to white
				Mat selectedPointImage = m->result.clone();
				Point p1 = Point(p.x - m->blocksize, p.y - m->blocksize);
				Point p2 = Point(p.x + m->blocksize, p.y + m->blocksize);
				rectangle(selectedPointImage, p1, p2, Scalar(255), 2, LINE_4, 0);
				imshow(HARRIS_CORNERS_WINDOW, selectedPointImage);
				// Selecting the region of interest from original picture
				Rect roi = Rect(p1.x, p1.y, m->blocksize*2, m->blocksize*2);
				Mat sub = Mat(m->source.clone(), roi);
				// Histogram from selected ROI
				m->CalculateHistogram(sub);
				selectedPointImage.deallocate();
				sub.deallocate();
				break;
			}
		}
	}
}

void HarrisCorners::BindMouseCallback() {
	setMouseCallback(HARRIS_CORNERS_WINDOW, MouseCallback, this);
}
