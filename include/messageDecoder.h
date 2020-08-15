#pragma once
#ifndef MESSAGE_DECODER_H
#define MESSAGE_DECODER_H
#include <iostream>
#include <vector>
#include <map>

class messageDecoder
{
public:
	messageDecoder();
	void printEyeStatesStream();
	void getState(int state);
	void displayMessage();
	void printDictionary();
	void initializeDictionary();

private:
	int closedEyeFrameCount;
	int openEyeFrameCount;
	bool shortBlinkBipFlag;
	bool longBlinkBipFlag;
	std::vector<int> eyeStatesStream;
	std::vector<std::string> messageHistory;
	std::string message;
	std::map<std::string, std::string> dictionary;
};
#endif // !MESSAGE_DECODER_H
