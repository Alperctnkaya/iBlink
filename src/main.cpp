#include <iostream>
#include "../include/detector.h"
#include "../include/messageDecoder.h"
#include <math.h>

using namespace std;
using namespace cv;

void printUsage(messageDecoder decoder) {
	cout << "0 Means short blink - 1 Means long blink" << endl;
	cout << "There is a bip sound indicates that blink is either short or long" << endl;
	cout << "if you open your eyes after first bip it is short blink" << endl;
	cout << "But if you wait for second bip sound to open your eyes it is long blink" << endl;
	decoder.printDictionary();
}

int main() {
	int x;
	cv::Mat capturedFrame;
	cv::VideoCapture capture(0); 
	detector detector; 
	messageDecoder decoder;
	printUsage(decoder);

	while (capture.read(capturedFrame)) {

		if (detector.detectFaceFeatures(capturedFrame))
			decoder.getState(detector.detectEyeState());

		if (waitKey(10) == 27)
			break;
	}

	return 0;
}
