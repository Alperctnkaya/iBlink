#include "../include/detector.h"


detector::detector() : centerface("models/centerface.onnx", 640, 480) {};

detector::detector(std::string ModelPath) : centerface(ModelPath, 640, 480) {

}

void detector::displayExtractedFeatures() {
	cv::imshow("face", this->face[0]);

}

bool detector::detectFaceFeatures(cv::Mat frame) {
	this->frame = frame.clone(); // clone frame to store unchanged frame incase further processing is needed

	// previous frame's regions not needed
	this->face.clear();
	this->eyes.clear();
	this->faceInfo.clear();

	cv::Point eye_top_left, eye_bottom_right; // points that specifies rectangular region around detected eye

	int scale; // scaling factor to specify eye region according detected face' size

	centerface.detect(frame, faceInfo); // detects face features and from given frame using specified model and stores features information in faceInfo vector

	//loops through each face detected in frame
	for (int i = 0; i < faceInfo.size(); i++) {
		face.push_back(this->frame(cv::Rect(faceInfo[i].x1, faceInfo[i].y1, faceInfo[i].x2 - faceInfo[i].x1, faceInfo[i].y2 - faceInfo[i].y1)));
		cv::rectangle(frame, cv::Point(faceInfo[i].x1, faceInfo[i].y1), cv::Point(faceInfo[i].x2, faceInfo[i].y2), cv::Scalar(0, 255, 0), 2);

		//loops throug detected face features in face -- 0- left eye, 1-right eye, 2 nose, 3 left side of mouth, 4 right side of mouth
		for (int j = 0; j < 2; j++) {

			scale = (faceInfo[i].x2 - faceInfo[i].x1) / 15; //Scale factor for different face sizes to seperate eye region

			//faceInfo[i].landmarks[2 * j] -> x coordinate of the feature, faceInfo[i].landmarks[2 * j + 1] -> y coordinate
			eye_top_left = cv::Point(faceInfo[i].landmarks[2 * j] - scale, faceInfo[i].landmarks[2 * j + 1] + scale);
			eye_bottom_right = cv::Point(faceInfo[i].landmarks[2 * j] + scale, faceInfo[i].landmarks[2 * j + 1] - scale);

			eyes.push_back(this->frame(cv::Rect(eye_top_left, eye_bottom_right)));

			//imshow("eye" + std::to_string(j), eyes[j+(j+2)*i]);

			cv::rectangle(frame, eye_top_left, eye_bottom_right, cv::Scalar(0, 255, 0), 2);
			cv::circle(frame, cv::Point(faceInfo[i].landmarks[2 * j], faceInfo[i].landmarks[2 * j + 1]), 2, cv::Scalar(255, 0, 0), 2);

		}
	}
	if (this->face.empty())
		return false;
	else
		return true;

}

int detector::detectEyeState() {

	if (this->eyes.empty())
		return -1;

	cv::Mat gray, binary, histEq, blured;
	cv::Mat mask, maskClone;

	int radius = this->eyes[0].rows / 5; //radius proportional to the size of the eye frame
	static const double PI = 3.1415;
	int eyeStates[2] = {};

	int blackPixels;
	int maxbPCount;

	mask = cv::Mat(this->eyes[0].rows, this->eyes[0].cols, CV_8U, cv::Scalar(127));

	for (int i = 0; i < this->eyes.size(); i++) {
		maxbPCount = 0;
		cv::cvtColor(~this->eyes[i], gray, cv::COLOR_RGB2GRAY); // cpnvert to grayscale
		cv::equalizeHist(gray, histEq); // histogram equalization to better detect the pupil regardless of ambient light
		cv::GaussianBlur(histEq, blured, cv::Size(5, 5), 1, 1); // gaussian blur to reduce white pixel around pupil
		cv::threshold(blured, binary, 220, 255, cv::THRESH_BINARY_INV); // thresholding to obtain only pupil 

		cv::imshow("eye" + (char)(i), binary);

		for (int x = 0; x < eyes[i].rows; x++) {
			for (int y = 0; y < eyes[i].cols; y++) {
				//mask which is the same size as the eye frame
				maskClone = ~mask.clone();

				//black circle inside white mask frame with radius proportional to the size of the eye frame loops through the eye frame 
				//and bitwise xor operation with the mask and binary image performed to count black pixels inside circle
				cv::circle(maskClone, cv::Point(x, y), radius, 0, -1);
				cv::bitwise_xor(maskClone, binary, maskClone);

				blackPixels = 0;

				//can be improved to loop through only circular area
				for (int q = 0; q < eyes[i].rows; q++) {
					for (int p = 0; p < eyes[i].cols; p++) {
						if ((int)maskClone.at<uchar>(q, p) == 0)
							blackPixels++;
					}

				}

				//finds the maximum black pixels within a circular area with a given radius in the eye frame
				if (blackPixels > maxbPCount)
					maxbPCount = blackPixels;
			}
		}

		//if the maximum black pixels count is more than 0.75 of the circular area, it means circular pupil is detected and eye is open
		if (maxbPCount > (PI * radius * radius) * 0.75)
			eyeStates[i] = 1;
		else
			eyeStates[i] = 0;

		//std::cout << "i: " << i << " MaxbP " << maxbPCount << " Eye State: " << eyeStates[i] << std::endl;
	}
	//std::cout << std::endl;
//std::cout <<"State: "<< eyeStates[0] + eyeStates[1] << std::endl;

	if ((eyeStates[0] + eyeStates[1]) == 0)  //both eyes are closed
		return 1;
	else
		return 0;
}