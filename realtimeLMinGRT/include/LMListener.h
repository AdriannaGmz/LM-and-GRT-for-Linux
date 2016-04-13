/******************************************************************************\
* 2015-2016     CINVESTAV   -  D. Adriana Gómez Rosal.    All rights reserved. *
* Adriana Gómez proprietary and confidential. Not for distribution.. yet.      *
\******************************************************************************/

#pragma once
#ifndef LMLISTENER_H
#define LMLISTENER_H

#include "Leap.h"
#include "LeapMath.h"
#include <fstream>  //for i/o file

using namespace Leap;

class LMListener : public Listener {
public:
	LMListener(void);
	~LMListener(void);
	virtual void onInit(const Controller&);
	virtual void onConnect(const Controller&);
	virtual void onDisconnect(const Controller&);
	virtual void onExit(const Controller&);
	virtual void onFocusGained(const Controller&);
	virtual void onFocusLost(const Controller&);
	virtual void onDeviceChange(const Controller&);
	virtual void onServiceConnect(const Controller&);
	virtual void onServiceDisconnect(const Controller&);
	virtual void onFrame(const Controller&);					//the Important one!!

	const float clickActivationDistance = 40;					//distance between fingers so it can be seen as a click
	const float extendedFingersDistance = 20;					//distance between fingers on PointerHand (Extended or not?)

protected:
	bool leftHanded; // user setting, flag for left and right-handed people
	int32_t clickerHandID, pointerHandID; // used to keep track of which detected hand from a given frame corresponds to the pointer and clicker
	bool rightHandFacesUp = false;
	bool leftHandFacesUp = false;
	std::ofstream file;
};

#endif
