#pragma once
#ifndef DETECTOR_H
#define DETECTOR_H
#include "cv_dnn_centerface.h"


class detector
{
public:
	
	detector();
	detector(std::string ModelPath);
	void displayExtractedFeatures();
	bool detectFaceFeatures(cv::Mat& Frame);
	int detectEyeState();

private:
	cv::Mat frame;
	std::vector<cv::Mat> face, eyes;
	std::vector<cv::Rect> eyesCoordinates;
	std::vector<FaceInfo> faceInfo;
	Centerface centerface;

};

#endif 
