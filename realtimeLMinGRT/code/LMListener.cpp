/******************************************************************************\
* 2015-2016     CINVESTAV   -  D. Adriana Gómez Rosal.    All rights reserved. *
* Adriana Gómez proprietary and confidential. Not for distribution.. yet.      *
\******************************************************************************/

#include <iostream>  
#include <string>
#include "Leap.h"
#include "LeapMath.h"
#include "LMListener.h"
#include <cstdlib>	
#include <fstream>  

using namespace Leap;

LMListener::LMListener(void) : leftHanded(false), clickerHandID(0), pointerHandID(0), rightHandFacesUp(false), leftHandFacesUp(false) {}		//constructor
LMListener::~LMListener(void) {}	//destructor

void LMListener::onInit(const Controller& controller) {
	std::cout << "Initialized" << std::endl;
    file.open("../dataFromLM.csv");      //open a file named dataFromLM.csv
}

void LMListener::onConnect(const Controller& controller) {
	std::cout << "Connected" << std::endl;
}

void LMListener::onDisconnect(const Controller& controller) {
	std::cout << "Disconnected" << std::endl;
}

void LMListener::onExit(const Controller& controller) {
	std::cout << "Exit" << std::endl;
	file.close();               //close the file
}

void LMListener::onFocusGained(const Controller& controller) {
	std::cout << "Focus Gained" << std::endl;
}

void LMListener::onFocusLost(const Controller& controller) {
	std::cout << "Focus Lost" << std::endl;
}

void LMListener::onDeviceChange(const Controller& controller) {
	std::cout << "Device Changed" << std::endl;
}

void LMListener::onServiceConnect(const Controller& controller) {
	std::cout << "Service Connected" << std::endl;
}

void LMListener::onServiceDisconnect(const Controller& controller) {
	std::cout << "Service Disconnected" << std::endl;
}

void LMListener::onFrame(const Controller& controller) {
	const Frame frame = controller.frame();	
	if (frame.hands().isEmpty()) return;	//do nothing unless hands are detected

	//	--Find the first hand and show position of fingers
	const Hand 			firstHand 			= frame.hands()[0];
	//const PointableList pointablesFirstHand = firstHand.pointables();	
	//const Pointable 	firstPointable 		= pointablesFirstHand[0];		
	//const FingerList 	fingers 			= firstHand.fingers();		
	/*
	for (int i = 0; i < fingers.count(); i++) 
	{
		const Finger finger = fingers[i];
		std::cout << "Finger " << i << " at position (" <<
			finger.tipPosition().x << ", " <<
			finger.tipPosition().y << ", " <<
			finger.tipPosition().z << ")" << std::endl;
	}
	*/


	//-- Write to the file.  Label 1 for all fingers separated among them. Label 2 for at least one of them is next-to another
	if (firstHand.isValid())
	{	
		std::string savingDistances="";
		const PointableList pointerFingers = firstHand.pointables();
		int extendedFingers=0;
		for (int i = 0; i < 4; i++)  // 4 distances between the 5 fingers
		{
			float pointFingerDist = pointerFingers[i].tipPosition().distanceTo(pointerFingers[i+1].tipPosition());
			if (pointFingerDist > extendedFingersDistance)
				extendedFingers++;
			// For the CSV format, each line needs to end without a coma:  label,data1,data2,data3,data4  and so on
			if (i!=3)
				savingDistances.append(std::to_string(pointFingerDist) + ",");
			else
				savingDistances.append(std::to_string(pointFingerDist));
		}

		//If the 4 distances are greater than the standard, assign Label..
		if (extendedFingers==4)
		{
			file << "1," << savingDistances << "\n";
			std::cout << "Extended Hand" << "\n";
		}
		else
		{
			file << "2," << savingDistances << "\n";	
			std::cout << "Non-Extended Hand" << "\n";
		}

	}
	
}
