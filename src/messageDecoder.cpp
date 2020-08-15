#include "../include/messageDecoder.h"


messageDecoder::messageDecoder() {
	initializeDictionary();
	this->closedEyeFrameCount = 0;
	this->openEyeFrameCount = 0;
	this->message = "";
	this->shortBlinkBipFlag = false;
	this->longBlinkBipFlag = false;
	std::cout << "Decoder Created" << std::endl;
}

void messageDecoder::printEyeStatesStream() {
	for (int i = 0; i < this->eyeStatesStream.size(); i++) {
		std::cout << this->eyeStatesStream[i] << " ";
	}
	std::cout << std::endl;
}

void messageDecoder::getState(int state) {

	if (state == 1) {
		this->closedEyeFrameCount++;
		this->openEyeFrameCount = 0;

		if ((this->closedEyeFrameCount > 3) && (! this->shortBlinkBipFlag)) {
			std::cout << '\a';
			this->shortBlinkBipFlag = true;
		}
		else if ((this->closedEyeFrameCount > 8) && (! this->longBlinkBipFlag)) {
			std::cout << '\a';
			this->longBlinkBipFlag = true;
		}
	}
	else if (state == 0) {
		
		this->closedEyeFrameCount = 0;
		this->openEyeFrameCount++;

		if (this->openEyeFrameCount > 2) {
			if (this->longBlinkBipFlag) {
				this->message += '1';
			}
			else if (this->shortBlinkBipFlag) {
				this->message += '0';
			}

			this->longBlinkBipFlag = false;
			this->shortBlinkBipFlag = false;
		}

		if (this->message.size() == 3) {
			std::cout << this->message << std::endl;
			displayMessage();
			this->messageHistory.push_back(message);
			this->message = "";
		}
	
	
	}

	if (this->eyeStatesStream.size() > 1000)
		this->eyeStatesStream.clear();

	this->eyeStatesStream.push_back(state);
	std::cout << "Open Frame Count : " << this->openEyeFrameCount << std::endl;
}

void messageDecoder::displayMessage() {
	std::cout << "MESSAGE: " << this->dictionary[this->message] << std::endl;
}

void messageDecoder::printDictionary() {
	for (auto i : this->dictionary ) {
		std::cout << i.second << " " << i.first << std::endl;
	}
}

void messageDecoder::initializeDictionary() {
	this->dictionary = { 
		{"000","A"},
		{"001","B"},
		{"010","C"},
		{"011","D"},
		{"100","E"},
		{"101","F"},
		{"110","G"},
		{"111","H"}		
						};
}