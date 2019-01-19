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
	this->histMode = 1;
	this->histCompSensitivity = 0.5f;
	this->currentPointsCount = 0;
}

HarrisCorners::~HarrisCorners() {

}

void HarrisCorners::DrawRectangle(Mat &frame, Point p, Scalar color) {
	Point p1 = Point(p.x - this->blocksize, p.y - this->blocksize);
	Point p2 = Point(p.x + this->blocksize, p.y + this->blocksize);
	rectangle(frame, p1, p2, color, 2, LINE_4, 0);
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
		this->DrawRectangle(this->result, this->pointsTab[i], Scalar(0));
	}
	std::cout << "Found points count: " << this->currentPointsCount << std::endl;
	// Showing the result
	imshow(HARRIS_CORNERS_WINDOW, this->result);
}

void HarrisCorners::CompareCorners(Mat frame) {
	this->FindCorners(frame);
	std::cout << "Current histograms comparator sensitivity: " << this->histCompSensitivity << std::endl;
	// when no points or too many points detected
	if(this->currentPointsCount == 0)
		return;

	// creating a vector with all detected points histograms and its points
	typedef struct {
		Mat hist;
		Point point;
	} histAtPoint_t;
	std::vector<histAtPoint_t> histTab;
	Mat source = frame.clone();
	Mat sub;
	Mat hist;
	int omittedPointsCount = 0;
	for(int i = 0; i < this->currentPointsCount; i++) {
		Point p = this->pointsTab[i];
		Point p1 = Point(p.x - this->blocksize, p.y - this->blocksize);
		// Selecting the region of interest from original picture
		Rect roi = Rect(p1.x, p1.y, this->blocksize*2, this->blocksize*2);
		try {
			sub = Mat(source, roi);
		} catch(cv::Exception& e) {
			// when incorrect point is detected
			omittedPointsCount++;
			continue;
		}
		// Histogram from selected ROI
		this->CalculateHistogram(sub, hist, false);
		histAtPoint_t data = {
				.hist = hist.clone(),
				.point = p
		};
		histTab.push_back(data);
	}
	// creating a vectors of similar points
	std::vector<std::vector<Point>> similarPointsVectors;
	while(0 < histTab.size()) {
		std::vector<Point> similarPointsTab;
		Mat x1 = histTab[0].hist.clone();
		int end = histTab.size();
		for(int i = 0; i < end;) {
			double correlation = compareHist(x1, histTab[i].hist, HISTCMP_CORREL);
			if(this->histCompSensitivity <= correlation) {
				similarPointsTab.push_back(histTab[i].point);
				histTab.erase(histTab.begin() + i);
				end--;
			} else
				i++;
		}
		similarPointsVectors.push_back(similarPointsTab);
	}
	// drawing similar points with different colors using 'Sinebow'
	int vectorsCount = similarPointsVectors.size();
	// In "ths->result" we have gray picture with found corners
	this->result = frame.clone();
	for(int i = 0; i < vectorsCount; i++) {
		Point3f color = Point3f(255.0f*(cosf(CV_PI*4.0f/3.0f + CV_2PI*(i/(float)vectorsCount)) + 1.0f)/2.0f,
								255.0f*(cosf(CV_PI*2.0f/3.0f + CV_2PI*(i/(float)vectorsCount)) + 1.0f)/2.0f,
								255.0f*(cosf(CV_2PI*(i/(float)vectorsCount)) + 1.0f)/2.0f);
		int pointsCount = similarPointsVectors[i].size();
		for(int j = 0; j < pointsCount; j++) {
			this->DrawRectangle(this->result, similarPointsVectors[i][j], Scalar((int)color.x, (int)color.y, (int)color.z));
		}
	}
	source.deallocate();
	sub.deallocate();
	hist.deallocate();
	// Showing the result
	imshow(MAIN_WINDOW_NAME, this->result);
}

void HarrisCorners::DestroyCornersWindow() {
	destroyWindow(HARRIS_CORNERS_WINDOW);
	this->currentPointsCount = 0;
}

void HarrisCorners::CalculateHistogram(Mat& frame, Mat& hist, bool ifShow) {
	cvtColor(frame, frame, COLOR_BGR2GRAY);
    // Set histogram bins count
    int bins = 256;
    int histSize[] = { bins };
    // Set ranges for histogram bins
    float lranges[] = { 0, 256 };
    const float* ranges[] = { lranges };
    // create matrix for histogram
    int channels[] = { 0 };
    // create matrix for histogram visualization
    int const hist_height = bins;
    Mat3b hist_image = Mat3b::zeros(hist_height, bins);
    // calculate histogram
    calcHist(&frame, 1, channels, cv::Mat(), hist, 1, histSize, ranges, true, false);

    if(ifShow) {
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
 }

void HarrisCorners::MouseCallback(int  event, int  x, int  y, int  flag, void *param) {
	HarrisCorners *m = (HarrisCorners *)param;
	if(event == EVENT_LBUTTONUP) {
		for(int i = 0; i < m->currentPointsCount; i++) {
			Point p = m->pointsTab[i];
			if(abs(p.x - x) < m->blocksize && abs(p.y - y) < m->blocksize) {
				// Painting selected rectangle to white
				Mat selectedPointImage = m->result.clone();
				cvtColor(selectedPointImage, selectedPointImage, COLOR_GRAY2BGR);
				m->DrawRectangle(selectedPointImage, p, Scalar(255, 255, 0));
				imshow(HARRIS_CORNERS_WINDOW, selectedPointImage);
				// Selecting the region of interest from original picture
				Point p1 = Point(p.x - m->blocksize, p.y - m->blocksize);
				Rect roi = Rect(p1.x, p1.y, m->blocksize*2, m->blocksize*2);
				Mat sub;
				Mat hist;
				try {
					sub = Mat(m->source.clone(), roi);
				} catch(cv::Exception& e) {
					std::cerr << "Invalid point selected!" << std::endl;
				    goto exit;
				}
				// Histogram from selected ROI
				m->CalculateHistogram(sub, hist, true);
			exit:
				selectedPointImage.deallocate();
				sub.deallocate();
				hist.deallocate();
				break;
			}
		}
	}
}

void HarrisCorners::BindMouseCallback() {
	setMouseCallback(HARRIS_CORNERS_WINDOW, MouseCallback, this);
}

void HarrisCorners::UnbindMouseCallback() {
	setMouseCallback(HARRIS_CORNERS_WINDOW, NULL, NULL);
}
